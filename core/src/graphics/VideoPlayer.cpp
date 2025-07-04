#include "pixl/core/graphics/VideoPlayer.h"
#include "pixl/core/asset/AssetManager.h"
#include "pixl/core/window/Window.h"

#include <glad/glad.h>
#include <thread>
#include <chrono>
#include <algorithm>
#include <AL/al.h>
#include <AL/alc.h>
#include <mutex>
#include <queue>
#include <vector>

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libavutil/frame.h>
    #include <libavutil/time.h>
    #include <libswresample/swresample.h>
    #include <libavutil/opt.h>
}

using namespace px;

extern px::WINDOW __pixl_rootwnd;

static std::vector<VIDPLAYER> __pixl_vid_managed;

void __pixl_vid_destroy_managed()
{
    for (VIDPLAYER player : __pixl_vid_managed)
    {
        delete player;
    }
}

/**
 * Video Player architecture:
 * 
 * VideoPlayer -> VideoPlayerImpl (updates) -> FrameHandler && AudioHandler
 *                      |
 *                     \/
 *                AVFrameProvider (supplies AVPackets to) -> FrameHandler && AudioHandler
 * 
 * FrameHandler (supplies textures to) -> VideoPlayerImpl
 * 
 * AudioHandler plays audio by itself
*/

#define AV_CALL(call) { int ret = call; if (ret != 0) { char errbuf[AV_ERROR_MAX_STRING_SIZE]; av_strerror(ret, errbuf, sizeof(errbuf)); Error::Throw(PX_ERROR_INTERNAL, std::string("libav: ") + errbuf); } }
#define AV_CALLr(call, v) { int ret = call; if (ret != 0) { char errbuf[AV_ERROR_MAX_STRING_SIZE]; av_strerror(ret, errbuf, sizeof(errbuf)); Error::Throw(PX_ERROR_INTERNAL, std::string("libav: ") + errbuf); return v; } }
#define AV_CALLrf(call, v, f) { int ret = call; if (ret != 0) { char errbuf[AV_ERROR_MAX_STRING_SIZE]; av_strerror(ret, errbuf, sizeof(errbuf)); Error::Throw(PX_ERROR_INTERNAL, std::string("libav: ") + errbuf); f return v; } }

#define QUEUE_SIZE 64
#define AUDIO_BUFFER_COUNT 8

static inline double NOW()
{
    return std::chrono::duration<double>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

static int AVIOReadPacket(void* opaque, uint8_t* buf, int buf_size) {
    std::istream* stream = (std::istream*)opaque;
    if (!stream || !stream->good()) return AVERROR_EOF;

    stream->read(reinterpret_cast<char*>(buf), buf_size);
    std::streamsize bytesRead = stream->gcount();

    if (bytesRead <= 0) return AVERROR_EOF;

    return bytesRead;
}

static int64_t AVIOSeekPacket(void* opaque, int64_t offset, int whence) {
    std::istream* stream = (std::istream*)opaque;
    if (!stream) return -1;

    std::ios_base::seekdir dir;
    switch (whence) {
        case SEEK_SET: dir = std::ios::beg; break;
        case SEEK_CUR: dir = std::ios::cur; break;
        case SEEK_END: dir = std::ios::end; break;
        case AVSEEK_SIZE:
        {
            auto currentPos = stream->tellg();
            stream->seekg(0, std::ios::end);
            auto size = stream->tellg();
            stream->seekg(currentPos);
            return size;
        }
        default: return -1;
    }

    stream->clear();
    stream->seekg(offset, dir);
    if (!stream->good()) return -1;

    return stream->tellg();
}

namespace px
{
    struct VideoPlayerInitData
    {
        double duration;
        uint8_t frameBufferSize;
        std::unique_ptr<std::istream> stream;
        AVIOContext* ioCtx = nullptr;
        AVFormatContext* ctx = nullptr;
        AVCodecContext* videoCtx = nullptr;
        AVCodecContext* audioCtx = nullptr;
        AVStream* videoStream = nullptr;
        AVStream* audioStream = nullptr;
    };

    struct Frame
    {
        GLuint tex;
        double time;
    };

    class AVFrameProvider
    {
    private:
        AVFormatContext* m_FmtCtx;
        AVCodecContext* m_VideoCtx;
        AVCodecContext* m_AudioCtx;
        AVStream* m_VideoStream;
        AVStream* m_AudioStream;

        std::queue<AVPacket*> m_VideoQueue;
        std::queue<AVPacket*> m_AudioQueue;
        
        volatile bool m_Running = true;
        std::thread m_ReadThread;
    public:
        AVFrameProvider(AVFormatContext* fmtCtx, AVCodecContext* videoCtx, AVCodecContext* audioCtx, AVStream* videoStream, AVStream* audioStream) : m_FmtCtx(fmtCtx), m_VideoCtx(videoCtx), m_AudioCtx(audioCtx), m_VideoStream(videoStream), m_AudioStream(audioStream)
        {
            while (m_VideoQueue.size() < QUEUE_SIZE && m_AudioQueue.size() < QUEUE_SIZE)
            {
                AVPacket* packet = av_packet_alloc();

                av_read_frame(m_FmtCtx, packet);

                if (packet->stream_index == m_VideoStream->index)
                {
                    m_VideoQueue.push(packet);
                }
                else if (packet->stream_index == m_AudioStream->index)
                {
                    m_AudioQueue.push(packet);
                }
            }
            
            m_ReadThread = std::thread([&]{ReadFrames();});

        }

        ~AVFrameProvider()
        {
            m_Running = false;
            m_ReadThread.join();

            while (m_VideoQueue.size() > 0)
            {
                AVPacket* packet = m_VideoQueue.front();
                m_VideoQueue.pop();
                av_packet_free(&packet);
            }

            while (m_AudioQueue.size() > 0)
            {
                AVPacket* packet = m_AudioQueue.front();
                m_AudioQueue.pop();
                av_packet_free(&packet);
            }
        }

        void ReadFrames()
        {
            while (m_Running)
            {
                if (m_VideoQueue.size() < QUEUE_SIZE || m_AudioQueue.size() < QUEUE_SIZE)
                {
                    AVPacket* packet = av_packet_alloc();

                    if (av_read_frame(m_FmtCtx, packet) != 0)
                    {
                        av_packet_free(&packet);
                        continue;
                    }


                    if (packet->stream_index == m_VideoStream->index)
                    {
                        m_VideoQueue.push(packet);
                    }
                    else if (packet->stream_index == m_AudioStream->index)
                    {
                        m_AudioQueue.push(packet);
                    }
                }
            }
        }

        AVPacket* GetVideoFrame()
        {
            if (m_VideoQueue.size() < 1) return nullptr;
            AVPacket* packet = m_VideoQueue.front();
            m_VideoQueue.pop();
            return packet;
        }

        AVPacket* GetAudioFrame()
        {
            if (m_AudioQueue.size() < 1) return nullptr;
            AVPacket* packet = m_AudioQueue.front();
            m_AudioQueue.pop();
            return packet;
        }
    };

    class FrameHandler
    {
    private:
        AVFrameProvider* m_FrameProvider;

        volatile bool m_Running = true;

        AVFormatContext* m_FmtCtx;
        AVCodecContext* m_Ctx;
        AVStream* m_Stream;

        AVPacket* m_Packet;
        AVFrame* m_Frame;
        AVFrame** m_RGBFrames;

        struct SwsContext* m_SwsCtx;

        GLuint m_TexturesCount;
        Frame* m_Textures;
        Frame* m_Textures1;
        volatile Frame* m_RenderingTextures;
        volatile Frame* m_DecodingTextures;
        volatile bool m_ReadyToUpload = false;
        GLuint m_TexturesRenderedIndex = 0;
        volatile bool m_RefillBuffers = false;
        bool m_First = false;

        std::thread m_DecodeThread;
    public:
        FrameHandler(AVFormatContext* fmtCtx, AVCodecContext* ctx, AVStream* stream, uint8_t frameBufferSize, AVFrameProvider* frameProvider)
        {
            m_FrameProvider = frameProvider;
            m_FmtCtx = fmtCtx;
            m_Ctx = ctx;
            m_Stream = stream;

            m_Packet = av_packet_alloc();
            m_Frame = av_frame_alloc();

            m_SwsCtx = sws_getContext(m_Ctx->width, m_Ctx->height, m_Ctx->pix_fmt, __pixl_rootwnd->GetFixedSize().x, __pixl_rootwnd->GetFixedSize().y, AV_PIX_FMT_RGB24, SWS_FAST_BILINEAR, NULL, NULL, NULL);

            m_TexturesCount = frameBufferSize;
            
            m_RGBFrames = new AVFrame*[m_TexturesCount];
            for (GLuint i = 0; i < m_TexturesCount; i++)
            {
                m_RGBFrames[i] = av_frame_alloc();
            }

            m_Textures = new Frame[m_TexturesCount];
            m_Textures1 = new Frame[m_TexturesCount];

            for (GLuint i = 0; i < m_TexturesCount; i++)
            {
                glGenTextures(1, &m_Textures[i].tex);
            }

            for (GLuint i = 0; i < m_TexturesCount; i++)
            {
                glGenTextures(1, &m_Textures1[i].tex);
            }
            
            m_RenderingTextures = m_Textures;
            m_DecodingTextures = m_Textures1;

            DecodeFrames();

            m_DecodeThread = std::thread([&]{RunDecodingThread();});
        }

        ~FrameHandler()
        {
            m_Running = false;
            if (m_DecodeThread.joinable()) m_DecodeThread.join();

            for (GLuint i = 0; i < m_TexturesCount; i++)
            {
                glDeleteBuffers(1, &m_Textures[i].tex);
            }

            for (GLuint i = 0; i < m_TexturesCount; i++)
            {
                glDeleteBuffers(1, &m_Textures1[i].tex);
            }

            delete[] m_Textures;
            delete[] m_Textures1;

            sws_freeContext(m_SwsCtx);

            av_packet_free(&m_Packet);
            av_frame_free(&m_Frame);
            
            for (GLuint i = 0; i < m_TexturesCount; i++)
            {
                av_frame_free(&m_RGBFrames[i]);
            }
            delete[] m_RGBFrames;

            avcodec_close(m_Ctx);
        }

        void RunDecodingThread()
        {
            while (m_Running)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                if (!m_RefillBuffers) continue;
                DecodeFrames();
            }
        }

        void DecodeFrames()
        {
            if (m_ReadyToUpload) return;
            m_RefillBuffers = false;

            int decodedCount = 0;

            for (GLuint i = 0; i < m_TexturesCount; ++i)
            {
                bool frameDecoded = false;

                AVPacket* packet = m_FrameProvider->GetVideoFrame();
                if (!packet)
                {
                    PX_DEBUG_LOG("FrameHandler::DecodeFrames()", "Packet queue empty");
                    continue;
                }

                if (avcodec_send_packet(m_Ctx, packet) == 0)
                {
                    int ret = avcodec_receive_frame(m_Ctx, m_Frame);
                    if (ret == 0)
                    {
                        AVFrame* rgb = m_RGBFrames[i];
                        if (!rgb->data[0])
                        {
                            av_frame_unref(rgb);
                            rgb->format = AV_PIX_FMT_RGB24;
                            rgb->width = __pixl_rootwnd->GetFixedSize().x;
                            rgb->height = __pixl_rootwnd->GetFixedSize().y;
                            av_frame_get_buffer(rgb, 0);
                        }

                        sws_scale(
                            m_SwsCtx,
                            m_Frame->data, m_Frame->linesize,
                            0, m_Ctx->height,
                            rgb->data, rgb->linesize
                        );

                        rgb->best_effort_timestamp = m_Frame->best_effort_timestamp;

                        frameDecoded = true;
                    }
                }

                av_packet_free(&packet);

                if (!frameDecoded)
                {
                    PX_DEBUG_LOG("FrameHandler::DecodeFrames()", "Could not decode enough frames (%d/%d)", decodedCount, m_TexturesCount);
                    break;
                }

                decodedCount++;
            }

            m_ReadyToUpload = decodedCount > 0;
        }

        void Update()
        {
            if (!m_ReadyToUpload) return;

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            for (GLuint i = 0; i < m_TexturesCount; i++)
            {
                Frame& frame = (Frame&)m_DecodingTextures[i];

                glBindTexture(GL_TEXTURE_2D, frame.tex);
                
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

                frame.time = m_RGBFrames[i]->best_effort_timestamp * av_q2d(m_Stream->time_base);

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_RGBFrames[i]->width, m_RGBFrames[i]->height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_RGBFrames[i]->data[0]);
            }

            glBindTexture(GL_TEXTURE_2D, 0);

            m_ReadyToUpload = false;
            if (m_First)
            {
                m_First = false;
                std::swap(m_RenderingTextures, m_DecodingTextures);
                m_RefillBuffers = true;
            }
        }

        void AdvanceFrames()
        {
            m_TexturesRenderedIndex++;

            if (m_TexturesRenderedIndex >= m_TexturesCount && !m_RefillBuffers)
            {
                m_TexturesRenderedIndex = 0;
                m_RefillBuffers = true;
                std::swap(m_RenderingTextures, m_DecodingTextures);
            }
        }

        Frame& GetFrame()
        {
            if (m_TexturesRenderedIndex >= m_TexturesCount)
            {
                return (Frame&)m_RenderingTextures[m_TexturesCount - 1];
            }

            return (Frame&)m_RenderingTextures[m_TexturesRenderedIndex];
        }
    };

    class AudioHandler {
    public:
        bool m_Paused = false;
        double m_Time = 0;
    private:
        AVFrameProvider* m_FrameProvider;

        AVFormatContext* m_FmtCtx;
        AVCodecContext* m_Ctx;
        AVStream* m_Stream;

        AVPacket* m_Packet;
        AVFrame* m_Frame;
        AVFrame* m_DecodedFrame;

        SwrContext* m_SwrCtx = nullptr;

        ALuint m_Buffers[AUDIO_BUFFER_COUNT];
        ALuint m_Source;
    public:
        AudioHandler(AVFormatContext* fmtCtx, AVCodecContext* ctx, AVStream* stream, AVFrameProvider* frameProvider) : m_FrameProvider(frameProvider), m_FmtCtx(fmtCtx), m_Ctx(ctx), m_Stream(stream)
        {
            m_Packet = av_packet_alloc();
            m_Frame = av_frame_alloc();
            m_DecodedFrame = av_frame_alloc();

            alGenBuffers(AUDIO_BUFFER_COUNT, m_Buffers);
            alGenSources(1, &m_Source);

            m_SwrCtx = swr_alloc();

            av_opt_set_chlayout(m_SwrCtx, "in_chlayout", &m_Ctx->ch_layout, 0);
            av_opt_set_int(m_SwrCtx, "in_sample_rate", m_Ctx->sample_rate, 0);
            av_opt_set_sample_fmt(m_SwrCtx, "in_sample_fmt", (AVSampleFormat)m_Ctx->sample_fmt, 0);

            AVChannelLayout outLayout = AV_CHANNEL_LAYOUT_STEREO;
            av_opt_set_chlayout(m_SwrCtx, "out_chlayout", &outLayout, 0);
            av_opt_set_int(m_SwrCtx, "out_sample_rate", 44100, 0);
            av_opt_set_sample_fmt(m_SwrCtx, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);

            if (swr_init(m_SwrCtx) < 0) {
                Error::Throw(PX_ERROR_INTERNAL, "Failed to initialize resampling context! (swresample)");
                return;
            }

            for (ALuint i = 0; i < AUDIO_BUFFER_COUNT; i++) {
                DecodeAudio(m_Buffers[i]);
            }

            alSourceQueueBuffers(m_Source, AUDIO_BUFFER_COUNT, m_Buffers);

            alSourcef(m_Source, AL_GAIN, 0.085f);
        }

        ~AudioHandler() {
            alSourceStop(m_Source);
            alDeleteSources(1, &m_Source);
            alDeleteBuffers(AUDIO_BUFFER_COUNT, m_Buffers);

            av_packet_free(&m_Packet);
            av_frame_free(&m_Frame);
            av_frame_free(&m_DecodedFrame);

            if (m_SwrCtx) swr_free(&m_SwrCtx);
        }

        void DecodeAudio(ALuint buf) {
            bool frameDecoded = false;

            while (!frameDecoded) {
                AVPacket* packet = m_FrameProvider->GetAudioFrame();
                if (!packet)
                {
                    PX_DEBUG_LOG("AudioHandler::DecodeAudio()", "Packet queue empty");
                    return;
                }
                
                if (avcodec_send_packet(m_Ctx, packet) == 0) {
                    if (avcodec_receive_frame(m_Ctx, m_Frame) == 0) {
                        av_frame_unref(m_DecodedFrame);
                        m_DecodedFrame->ch_layout = AV_CHANNEL_LAYOUT_STEREO;
                        m_DecodedFrame->sample_rate = 44100;
                        m_DecodedFrame->format = AV_SAMPLE_FMT_S16;

                        if (swr_convert_frame(m_SwrCtx, m_DecodedFrame, m_Frame) < 0) {
                            PX_DEBUG_LOG("AudioHandler::DecodeAudio()", "Failed to resample audio frame");
                            av_packet_free(&packet);
                            continue;
                        }

                        int channels = m_DecodedFrame->ch_layout.nb_channels;
                        int bufferSize = av_samples_get_buffer_size(
                            NULL, channels, m_DecodedFrame->nb_samples,
                            (AVSampleFormat)m_DecodedFrame->format, 1
                        );

                        ALenum format = (channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
                        alBufferData(buf, format, m_DecodedFrame->data[0], bufferSize, 44100);

                        frameDecoded = true;
                    }
                }

                av_packet_free(&packet);
            }

            if (!frameDecoded) {
                PX_DEBUG_LOG("AudioHandler::DecodeAudio()", "Failed to decode audio frame");
            }
        }

        void Update(double clock) {
            ALint processed = 0;
            alGetSourcei(m_Source, AL_BUFFERS_PROCESSED, &processed);

            while (processed-- > 0) {
                ALuint buf;
                alSourceUnqueueBuffers(m_Source, 1, &buf);

                ALint size;
                alGetBufferi(buf, AL_SIZE, &size);
                m_Time += (double)size / (2.0 * 2.0 * 44100.0); // size / (channels * sample size in bytes * sample rate)
                
                DecodeAudio(buf);
                alSourceQueueBuffers(m_Source, 1, &buf);
            }

            ALint state;
            alGetSourcei(m_Source, AL_SOURCE_STATE, &state);
            if (state != AL_PLAYING && !m_Paused) {
                alSourcePlay(m_Source);
            }
        }

        void Start()
        {
            m_Paused = false;
            alSourcePlay(m_Source);
        }

        void Pause()
        {
            m_Paused = true;
            alSourcePause(m_Source);
        }
    };


    class VideoPlayerImpl
    {
    private:
        std::unique_ptr<std::istream> m_InputStream;
        AVFormatContext* m_Ctx;
        AVIOContext* m_IOCtx;
        AVFrameProvider* m_FrameProvider;
        FrameHandler* m_FrameHandler;
        AudioHandler* m_AudioHandler;
        double m_PlaybackClock;
        bool m_Playing = false;
        bool m_Stopped = false;
        std::mutex m_ReadMutex;
        double m_Duration;
    public:
        VideoPlayerImpl(VideoPlayerInitData& data)
        {
            m_InputStream = std::move(data.stream);
            m_Ctx = data.ctx;
            m_IOCtx = data.ioCtx;
            m_Duration = data.duration;

            m_FrameProvider = new AVFrameProvider(m_Ctx, data.videoCtx, data.audioCtx, data.videoStream, data.audioStream);
            m_FrameHandler = new FrameHandler(data.ctx, data.videoCtx, data.videoStream, data.frameBufferSize, m_FrameProvider);
            m_AudioHandler = new AudioHandler(data.ctx, data.audioCtx, data.audioStream, m_FrameProvider);
        }

        ~VideoPlayerImpl()
        {
            delete m_FrameHandler;
            delete m_AudioHandler;
            delete m_FrameProvider;

            avformat_close_input(&m_Ctx);
            avio_context_free(&m_IOCtx);
        }

        void Update(float delta)
        {
            if (m_Stopped) return;
            if (!m_Playing) return;

            m_PlaybackClock = m_AudioHandler->m_Time;
        
            if (m_PlaybackClock >= m_Duration)
            {
                m_Stopped = true;
                m_Playing = false;
                return;
            }

            m_FrameHandler->Update();
            m_AudioHandler->Update(m_PlaybackClock);
        }

        void Draw(const Mat4& mat, const DrawData& data)
        {
            if (m_Stopped) return;

            Frame& frame = m_FrameHandler->GetFrame();
            GLuint tex = frame.tex;

            if (frame.time <= m_PlaybackClock)
            {
                m_FrameHandler->AdvanceFrames();
            }

            data.shd->Use();

            data.shd->SetMatrix4("model_matrix", mat);
            data.shd->SetVec2("px_uv_coord", Vec2());
            data.shd->SetVec2("px_uv_size", Vec2(1.0f, 1.0f));
            data.shd->SetBool("px_flip_x", false);
            data.shd->SetBool("px_flip_y", false);
            data.shd->SetColor("px_color", Color());

            glBindTexture(GL_TEXTURE_2D, tex);

            data.ctx->DrawQuad();

            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void Play()
        {
            if (m_Stopped) return;
            m_PlaybackClock = 0.0;
            m_Playing = true;
            m_AudioHandler->Start();
        }

        void Pause()
        {
            if (m_Stopped) return;
            m_Playing = false;
            m_AudioHandler->Pause();
        }

        void Resume()
        {
            if (m_Stopped) return;
            m_Playing = true;
            m_AudioHandler->Start();
        }

        void Stop()
        {
            if (m_Stopped) return;
            m_Playing = false;
            m_Stopped = true;
            m_AudioHandler->Pause();
        }

        bool IsPlaying()
        {
            return m_Playing;
        }

        bool IsPaused()
        {
            return m_Playing && !m_Stopped;
        }

        bool IsFinished()
        {
            return m_Stopped;
        }
    };
}

px::VideoPlayer::VideoPlayer(VideoPlayerInitData& data) : size(__pixl_rootwnd->GetFixedSize())
{
    m_Impl = new VideoPlayerImpl(data);
}

px::VideoPlayer::~VideoPlayer()
{
    delete m_Impl;
}

void px::VideoPlayer::Play()
{
    m_Impl->Play();
}

void px::VideoPlayer::Pause()
{
    m_Impl->Pause();
}

void px::VideoPlayer::Resume()
{
    m_Impl->Resume();
}

void px::VideoPlayer::Stop()
{
    m_Impl->Stop();   
}

void px::VideoPlayer::Draw(const DrawData& data)
{
    Vec2 _pos = pos;

    Mat4 mat;
    Vec2 diff = Vec2((size.x * scale.x) - size.x, (size.y * scale.y) - size.y);

    mat.Translate(Vec2(_pos.x - (diff.x / 2.0f), _pos.y - (diff.y / 2.0f)));

    mat.Translate(Vec2(size.x * pivotPoint.x * scale.x, size.y * pivotPoint.y * scale.y));
    mat.Rotate(Vec3(0.0f, 0.0f, rotation));
    mat.Translate(Vec2(-(size.x * pivotPoint.x) * scale.x, -(size.y * pivotPoint.y) * scale.y));

    mat.Scale(Vec2(size.x + diff.x, size.y + diff.y));

    m_Impl->Draw(mat, data);
}

void px::VideoPlayer::Update(float delta)
{
    m_Impl->Update(delta);
}

bool px::VideoPlayer::IsPlaying()
{
    return m_Impl->IsPlaying();
}

bool px::VideoPlayer::IsPaused()
{
    return m_Impl->IsPaused();
}

bool px::VideoPlayer::IsFinished()
{
    return m_Impl->IsFinished();
}

VIDPLAYER px::VideoPlayer::Create(CREFSTR path, bool managed, uint8_t frameBufferSize)
{
    std::unique_ptr<std::istream> stream = AssetManager::GetStream(path);
    if (!stream)
    {
        Error::Throw(PX_ERROR_ASSET_NOT_AVAILABLE, "No file / stream found for: " + path);
        return nullptr;
    }

    uint8_t* avioBuffer = (uint8_t*)av_malloc(4096);
    AVIOContext* avioContext = avio_alloc_context(
        avioBuffer, 4096,
        0,
        stream.get(),
        &AVIOReadPacket,
        NULL,
        &AVIOSeekPacket
    );

    AVFormatContext* formatContext = avformat_alloc_context();
    formatContext->pb = avioContext;
    formatContext->flags |= AVFMT_FLAG_CUSTOM_IO;

    AV_CALLrf(avformat_open_input(&formatContext, NULL, NULL, NULL), nullptr, avformat_free_context(formatContext); avio_context_free(&avioContext););
    AV_CALLrf(avformat_find_stream_info(formatContext, NULL), nullptr, avformat_free_context(formatContext); avio_context_free(&avioContext););

    PX_DEBUG_LOG("VideoPlayer::Create()", "Video format: %s | Duration: %.3f seconds", formatContext->iformat->long_name, formatContext->duration / (double)AV_TIME_BASE);

    VideoPlayerInitData initData{};
    initData.duration = formatContext->duration / (double)AV_TIME_BASE;
    initData.frameBufferSize = frameBufferSize;
    initData.ioCtx = avioContext;
    initData.ctx = formatContext;

    for (unsigned int i = 0; i < formatContext->nb_streams; i++)
    {
        AVCodecParameters* codecParams = formatContext->streams[i]->codecpar;
        const AVCodec* codec = avcodec_find_decoder(codecParams->codec_id);
        if (!codec)
        {
            Error::Throw(PX_ERROR_INTERNAL, std::string("Decoder not available for: ") + avcodec_get_name(codecParams->codec_id));
            avformat_free_context(formatContext);
            avio_context_free(&avioContext);
            return nullptr;
        }
        else
        {
            PX_DEBUG_LOG("VideoPlayer::Create()", "Found decoder for: %s", codec->long_name);
        }

        PX_DEBUG_LOG("VideoPlayer::Create()", "Found stream: %s (%s)", av_get_media_type_string(codec->type), codec->long_name);

        if (initData.videoCtx && initData.audioCtx) break;
        
        AVCodecContext* codecCtx = avcodec_alloc_context3(codec);
        avcodec_parameters_to_context(codecCtx, codecParams);

        AV_CALLrf(avcodec_open2(codecCtx, codec, NULL), nullptr, avcodec_close(codecCtx); if (initData.videoCtx) avcodec_close(initData.videoCtx); if (initData.audioCtx) avcodec_close(initData.audioCtx); avformat_free_context(formatContext); avio_context_free(&avioContext););

        switch (codec->type)
        {
        case AVMEDIA_TYPE_VIDEO: initData.videoCtx = codecCtx; initData.videoStream = formatContext->streams[i]; break;
        case AVMEDIA_TYPE_AUDIO: initData.audioCtx = codecCtx; initData.audioStream = formatContext->streams[i]; break;
        default: continue;
        }
    }

    initData.stream = std::move(stream);

    VIDPLAYER player = new VideoPlayer(initData);
    if (managed) __pixl_vid_managed.push_back(player);
    return player;
}

void px::VideoPlayer::Destroy(VIDPLAYER player)
{
    int i = -1;
    for (VIDPLAYER plr : __pixl_vid_managed)
    {
        if (i < 0) i = 0;

        if (plr == player)
        {
            break;
        }

        i++;
    }

    if (i >= 0) __pixl_vid_managed.erase(__pixl_vid_managed.begin() + i);

    delete player;
}
