#include "pixl/core/window/Window.h"
#include "pixl/core/pixl.h"
#include "pixl/core/graphics/RenderPipeline.h"
#include "pixl/core/graphics/2d/CameraRenderer.h"
#include "pixl/core/graphics/2d/WindowRenderer.h"
#include "pixl/core/graphics/3d/Camera3DRenderer.h"
#include "pixl/core/input/Keys.h"
#include "pixl/core/input/Mouse.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>
#include <math.h>

extern px::InitOptions __pixl_opts;

using namespace px;

extern WINDOW __pixl_rootwnd;
extern bool __pixl_singlewnd;
extern void __pixl_timer_update(float delta);

static std::string GetDebugSource(GLenum source)
{
    switch (source)
    {
    case GL_DEBUG_SOURCE_API: return "API";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "Window System";
    case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
    case GL_DEBUG_SOURCE_THIRD_PARTY: return "Third Party";
    case GL_DEBUG_SOURCE_APPLICATION: return "Application";
    case GL_DEBUG_SOURCE_OTHER: return "Other";
    default: return "unknown source";
    }
}

static std::string GetDebugType(GLenum type)
{
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR: return "Error";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behavior";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "Undefined Behavior";
    case GL_DEBUG_TYPE_PORTABILITY: return "Portability";
    case GL_DEBUG_TYPE_PERFORMANCE: return "Performance";
    case GL_DEBUG_TYPE_OTHER: return "Other";
    case GL_DEBUG_TYPE_MARKER: return "Marker";
    case GL_DEBUG_TYPE_PUSH_GROUP: return "Push Group";
    case GL_DEBUG_TYPE_POP_GROUP: return "Pop Group";
    default: return "unknown type";
    }
}

static std::string GetDebugSeverity(GLenum severity)
{
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH: return "High";
    case GL_DEBUG_SEVERITY_MEDIUM: return "Medium";
    case GL_DEBUG_SEVERITY_LOW: return "Low";
    case GL_DEBUG_SEVERITY_NOTIFICATION: return "Notification";
    default: return "unknown severitry";
    }
}

void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    std::cout << "[pixl::OpenGL] [" << GetDebugSource(source) << ":" << GetDebugType(type) << "/" << GetDebugSeverity(severity) << "] (" << id << "): " << message << std::endl;
}

static void _glfw_framebuffer_resize(GLFWwindow* window, int width, int height);
static void _glfw_keycallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void _glfw_mousebuttoncallback(GLFWwindow* window, int button, int action, int mods);
static void _glfw_cursorposcallback(GLFWwindow* window, double xpos, double ypos);
static void _glfw_scrollcallback(GLFWwindow* window, double xoffset, double yoffset);

namespace px
{
    class WindowImpl
    {
    public:
        WINDOW m_Parent;
        GLFWwindow* m_Handle;
        RENDERPIPELINE m_Pipeline;
        std::vector<CAMERA> m_Cameras;
        std::vector<CAMERA> m_DrawCameras;
        DRAWINGCTX m_DrawingContext;
        Vec2i m_InitialSize;
        EventManager* m_EventManager;
        Vec2i m_ViewportPos;
        Vec2i m_ViewportSize;
        CAMERA m_StaticCamera;
        std::vector<CAMERA3D> m_Cameras3D;
        GLFWmonitor* m_Monitor;
        bool m_VSync = false;
    public:
        WindowImpl(WINDOW parent, CREFSTR title, const Vec2i& size, bool startVisible, bool decorated) : m_Parent(parent), m_InitialSize(size)
        {
            glfwDefaultWindowHints();
			glfwWindowHint(GLFW_VISIBLE, startVisible);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_SAMPLES, 4);
			glfwWindowHint(GLFW_DECORATED, decorated);
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

#ifdef PX_WIN
            m_Monitor = glfwGetPrimaryMonitor();
#elif defined(PX_UNIX)
            GLFWmonitor** monitors;
            int count;
            monitors = glfwGetMonitors(&count);
            m_Monitor = monitors[0];
#endif

            PX_DEBUG_LOG("WindowImpl::WindowImpl()", "Creating window: (parent: 0x%x) title: %s | width: %d | height: %d | startVisible: %s | decorated: %s", parent, title.c_str(), size.x, size.y, startVisible ? "true" : "false", decorated ? "true" : "false");
            PX_DEBUG_LOG("WindowImpl::WindowImpl()", "Is root window?: %s", !__pixl_rootwnd ? "Yes" : "No");

            if (__pixl_rootwnd)
            {
                __pixl_singlewnd = false;
            }

            m_Handle = glfwCreateWindow(size.x, size.y, title.c_str(), NULL, __pixl_rootwnd ? (GLFWwindow*)__pixl_rootwnd->GetHandle() : nullptr);
            if (!m_Handle)
            {
                Error::Throw(PX_ERROR_WINDOW_CREATE, "Failed to create window");
                return;
            }

			glfwMakeContextCurrent(m_Handle);
			glfwSwapInterval(0);

            glfwSetWindowUserPointer(m_Handle, this);
            glfwSetFramebufferSizeCallback(m_Handle, _glfw_framebuffer_resize);
            glfwSetKeyCallback(m_Handle, _glfw_keycallback);
            glfwSetMouseButtonCallback(m_Handle, _glfw_mousebuttoncallback);
            glfwSetCursorPosCallback(m_Handle, _glfw_cursorposcallback);
            glfwSetScrollCallback(m_Handle, _glfw_scrollcallback);

            m_ViewportSize = size;

            Center();

            if (!__pixl_rootwnd)
            {
                if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
                {
                    Error::Throw(PX_ERROR_OPENGL_LOAD, "Failed to load opengl");
                    return;
                }
            }

            if (!__pixl_rootwnd)
            {
                __pixl_rootwnd = parent;
            }

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(DebugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        ~WindowImpl()
        {
            for (CAMERA camera : m_Cameras)
            {
                delete camera;
            }

            for (CAMERA3D camera : m_Cameras3D)
            {
                delete camera;
            }

            delete m_StaticCamera;
            delete m_DrawingContext;
            delete m_Pipeline;
            delete m_EventManager;

            glfwDestroyWindow(m_Handle);
        }

        void PostInit()
        {
            m_EventManager = new EventManager;

            m_DrawingContext = new DrawingContext;

            m_Pipeline = m_Parent->CreateDefaultPipeline();

            m_StaticCamera = new Camera(m_Parent);
            AddCamera(new Camera(m_Parent)); // default camera

            m_Cameras3D.push_back(new Camera3D(m_Parent));

            WindowChangedData data{};
            data.viewportPos = { 0, 0 };
            data.viewportSize = m_InitialSize;
        
            m_EventManager->CallEvent(PX_EVENT(WINDOW_CHANGED), data);

            if (__pixl_rootwnd == m_Parent)
            {
                Keys::Init(m_Parent);
                Mouse::Init(m_Parent);
            }
        }

        void* GetHandle()
        {
            return m_Handle;
        }

        void SetVisible(bool visible)
        {
            if (visible)
            {
                glfwShowWindow(m_Handle);
            }
            else
            {
                glfwHideWindow(m_Handle);
            }
        }

        bool ShouldClose()
        {
            return glfwWindowShouldClose(m_Handle);
        }

        void Update(float delta)
        {
            if (!__pixl_singlewnd) glfwMakeContextCurrent(m_Handle);

            if (__pixl_rootwnd == m_Parent)
            {
                __pixl_timer_update(delta);
                Keys::Update();
                Mouse::Update();
            }

            m_StaticCamera->Update(delta);
            for (CAMERA camera : m_Cameras)
            {
                camera->Update(delta);
            }

            for (CAMERA3D camera : m_Cameras3D)
            {
                camera->Update(delta);
            }
        }

        void UpdateEvents(float delta)
        {
            glfwMakeContextCurrent(m_Handle);
            glfwPollEvents();
        }

        void Draw()
        {
            glfwMakeContextCurrent(m_Handle);

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            m_Pipeline->Flush();

            glfwSwapBuffers(m_Handle);
        }

        const std::vector<CAMERA> GetCameras()
        {
            return m_DrawCameras;
        }

        void AddCamera(CAMERA camera)
        {
            m_Cameras.push_back(camera);

            m_DrawCameras.clear();
            m_DrawCameras.assign(m_Cameras.begin(), m_Cameras.end());
            m_DrawCameras.push_back(m_StaticCamera);
        }

        void RemoveCamera(CAMERA camera)
        {
            auto it = std::find(m_Cameras.begin(), m_Cameras.end(), camera);
            if (it == m_Cameras.end()) return;
            m_Cameras.erase(it);

            m_DrawCameras.clear();
            m_DrawCameras.assign(m_Cameras.begin(), m_Cameras.end());
            m_DrawCameras.push_back(m_StaticCamera);
        }

        CAMERA GetDefaultCamera()
        {
            if (m_Cameras.empty()) return nullptr;
            return m_Cameras.front();
        }

        DRAWINGCTX GetDrawingContext()
        {
            return m_DrawingContext;
        }

        Vec2i GetFixedSize()
        {
            return m_InitialSize;
        }

        EVTMGR GetEventManager()
        {
            return m_EventManager;
        }

        void Close()
        {
            glfwSetWindowShouldClose(m_Handle, GLFW_TRUE);
        }

        void Center()
        {
            const GLFWvidmode* vidmode = glfwGetVideoMode(m_Monitor);
#ifdef PX_WIN
            glfwSetWindowPos(m_Handle, vidmode->width / 2 - m_InitialSize.x / 2, vidmode->height / 2 - m_InitialSize.y / 2);
#elif defined(PX_UNIX)
            int monitorX, monitorY;
            glfwGetMonitorPos(primaryMonitor, &monitorX, &monitorY);
            int windowX = monitorX + (vidmode->width - m_InitialSize.x) / 2;
            int windowY = monitorY + (vidmode->height - m_InitialSize.y) / 2;

            glfwSetWindowPos(m_Handle, windowX, windowY);
#endif
        }
    };
};

static void _glfw_framebuffer_resize(GLFWwindow* window, int width, int height)
{
    if (width < 1 || height < 1) return;

	WindowImpl* wnd = (WindowImpl*)glfwGetWindowUserPointer(window);
	Vec2i size = wnd->GetFixedSize();

	if (width < 1 || height < 1) return;

	float ratioX = width / (float)size.x;
	float ratioY = height / (float)size.y;
	float ratio = fminf(ratioX, ratioY);

	int viewWidth = (int)(size.x * ratio);
	int viewHeight = (int)(size.y * ratio);

	int viewX = (int)(width - size.x * ratio) / 2;
	int viewY = (int)(height - size.y * ratio) / 2;

    glViewport(viewX, viewY, viewWidth, viewHeight);

    wnd->m_ViewportPos = Vec2i(viewX, viewY);
    wnd->m_ViewportSize = Vec2i(viewWidth, viewHeight);

    WindowChangedData data{};
    data.viewportPos = { viewX, viewY };
    data.viewportSize = { viewWidth, viewHeight };

    wnd->GetEventManager()->CallEvent(PX_EVENT(WINDOW_CHANGED), data);
}

static void _glfw_keycallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    WindowImpl* wnd = (WindowImpl*)glfwGetWindowUserPointer(window);

    KeyEvent data{};
    data.code = (KeyCode)key;

    if (action == GLFW_PRESS)
    {
        wnd->GetEventManager()->CallEvent(PX_EVENT(KEY_PRESS), data);
    }
    else if (action == GLFW_RELEASE)
    {
        wnd->GetEventManager()->CallEvent(PX_EVENT(KEY_RELEASE), data);
    }
}

static void _glfw_mousebuttoncallback(GLFWwindow* window, int button, int action, int mods)
{
    WindowImpl* wnd = (WindowImpl*)glfwGetWindowUserPointer(window);

    MouseEvent data{};
    data.button = (MouseButton)button;

    if (action == GLFW_PRESS)
    {
        wnd->GetEventManager()->CallEvent(PX_EVENT(MOUSE_PRESS), data);
    }
    else if (action == GLFW_RELEASE)
    {
        wnd->GetEventManager()->CallEvent(PX_EVENT(MOUSE_RELEASE), data);
    }
}

static void _glfw_cursorposcallback(GLFWwindow* window, double xpos, double ypos)
{
    WindowImpl* wnd = (WindowImpl*)glfwGetWindowUserPointer(window);

    MouseMoveEvent data{};
    data.x = xpos;
    data.y = ypos;

    wnd->GetEventManager()->CallEvent(PX_EVENT(MOUSE_MOVE), data);
}

static void _glfw_scrollcallback(GLFWwindow* window, double xoffset, double yoffset)
{
    WindowImpl* wnd = (WindowImpl*)glfwGetWindowUserPointer(window);

    MouseScrollEvent data{};
    data.x = xoffset;
    data.y = yoffset;

    wnd->GetEventManager()->CallEvent(PX_EVENT(MOUSE_SCROLL), data);
}

px::Window::Window(CREFSTR title, const Vec2i& size, bool startVisible, bool decorated)
{
    m_Impl = new WindowImpl(this, title, size, startVisible, decorated);
}

px::Window::~Window()
{
    delete m_Impl;
}

void px::Window::SetVisible(bool visible)
{
    m_Impl->SetVisible(visible);
}

bool px::Window::ShouldClose()
{
    return m_Impl->ShouldClose();
}

const std::vector<CAMERA> px::Window::GetCameras()
{
    return m_Impl->GetCameras();
}

const std::vector<CAMERA3D> px::Window::GetCameras3D()
{
    return m_Impl->m_Cameras3D;
}

CAMERA3D px::Window::GetDefaultCamera3D()
{
    if (m_Impl->m_Cameras3D.empty()) return nullptr;
    return m_Impl->m_Cameras3D.front();
}

void px::Window::AddCamera3D(CAMERA3D camera)
{
    m_Impl->m_Cameras3D.push_back(camera);
}

void px::Window::RemoveCamera3D(CAMERA3D camera)
{
    auto it = std::find(m_Impl->m_Cameras3D.begin(), m_Impl->m_Cameras3D.end(), camera);
    if (it == m_Impl->m_Cameras3D.end()) return;
    m_Impl->m_Cameras3D.erase(it);
}

void px::Window::AddCamera(CAMERA camera)
{
    m_Impl->AddCamera(camera);
}

void px::Window::RemoveCamera(CAMERA camera)
{
    m_Impl->RemoveCamera(camera);
}

CAMERA px::Window::GetDefaultCamera()
{
    return m_Impl->GetDefaultCamera();
}

DRAWINGCTX px::Window::GetDrawingContext()
{
    return m_Impl->GetDrawingContext();
}

Vec2i px::Window::GetFixedSize()
{
    return m_Impl->GetFixedSize();
}

EVTMGR px::Window::GetEventManager()
{
    return m_Impl->GetEventManager();
}

Vec2i px::Window::GetViewportPos()
{
    return m_Impl->m_ViewportPos;
}

Vec2i px::Window::GetViewportSize()
{
    return m_Impl->m_ViewportSize;
}

Vec2i px::Window::GetPosition()
{
    Vec2i pos;
    glfwGetWindowPos(m_Impl->m_Handle, &pos.x, &pos.y);
    return pos;
}

Vec2i px::Window::GetSize()
{
    Vec2i size;
    glfwGetWindowSize(m_Impl->m_Handle, &size.x, &size.y);
    return size;
}

void px::Window::SetPosition(const Vec2i& pos)
{
    glfwSetWindowPos(m_Impl->m_Handle, pos.x, pos.y);
}

void px::Window::SetSize(const Vec2i& size)
{
    glfwSetWindowSize(m_Impl->m_Handle, size.x, size.y);
}

void px::Window::SetFullscreen(bool fullscreen)
{
    if (fullscreen)
    {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowMonitor(m_Impl->m_Handle, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, 0 );
    }
    else
    {
        glfwSetWindowMonitor(m_Impl->m_Handle, nullptr, 0, 0, m_Impl->m_InitialSize.x, m_Impl->m_InitialSize.y, GLFW_DONT_CARE);
        m_Impl->Center();
    }
}

RENDERPIPELINE px::Window::GetRenderPipeline()
{
    return m_Impl->m_Pipeline;
}

void px::Window::SetRenderPipeline(RENDERPIPELINE pipeline)
{
    if (m_Impl->m_Pipeline) delete m_Impl->m_Pipeline;
    m_Impl->m_Pipeline = pipeline;
    
    WindowChangedData data{};
    data.viewportPos = m_Impl->m_ViewportPos;
    data.viewportSize = m_Impl->m_ViewportSize;

    m_Impl->m_EventManager->CallEvent(PX_EVENT(WINDOW_CHANGED), data);
}

RENDERPIPELINE px::Window::CreateDefaultPipeline()
{
    RENDERPIPELINE pipeline = new RenderPipeline(this);
    pipeline->AddElement("cam", new CameraRenderer);
    pipeline->AddElement("wnd", new WindowRenderer);
    return pipeline;
}

RENDERPIPELINE px::Window::CreateDefault3DPipeline()
{
    RENDERPIPELINE pipeline = new RenderPipeline(this);
    pipeline->AddElement("3d", new Camera3DRenderer);
    pipeline->AddElement("cam", new CameraRenderer);
    pipeline->AddElement("wnd", new WindowRenderer);
    return pipeline;
}

CAMERA px::Window::GetStaticCamera()
{
    return m_Impl->m_StaticCamera;
}

void px::Window::Close()
{
    m_Impl->Close();
}

void px::Window::DisableCursor()
{
    glfwSetInputMode(m_Impl->m_Handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void px::Window::EnableCursor()
{
    glfwSetInputMode(m_Impl->m_Handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void px::Window::ShowCursor()
{
    EnableCursor();
}

void px::Window::SetVSync(bool enabled)
{
    m_Impl->m_VSync = enabled;
    glfwMakeContextCurrent(m_Impl->m_Handle);
    glfwSwapInterval(enabled);
}

void px::Window::HideCursor()
{
    glfwSetInputMode(m_Impl->m_Handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void px::Window::SetTitle(CREFSTR title)
{
    glfwSetWindowTitle(m_Impl->m_Handle, title.c_str());
}

std::string px::Window::GetTitle()
{
    return glfwGetWindowTitle(m_Impl->m_Handle);
}

void px::Window::Update(float delta)
{
    m_Impl->Update(delta);
}

void px::Window::UpdateEvents(float delta)
{
    m_Impl->UpdateEvents(delta);
}

void px::Window::Draw()
{
    m_Impl->Draw();
}

void* px::Window::GetHandle()
{
    return m_Impl->GetHandle();
}

void px::Window::PostInit()
{
    m_Impl->PostInit();
}
