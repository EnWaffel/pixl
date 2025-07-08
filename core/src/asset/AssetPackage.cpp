#include "pixl/core/asset/AssetPackage.h"

#include <filesystem>
#include <fstream>
#include <vector>

using namespace px;
namespace fs = std::filesystem;

PackageBuffer::PackageBuffer(std::streambuf* src, std::streampos startPos, std::streamsize size) : m_Source(src), m_Start(startPos), m_End(startPos + size) {
    m_Source->pubseekpos(m_Start);
}

std::streambuf::int_type PackageBuffer::underflow() {
    std::streampos current = m_Source->pubseekoff(0, std::ios_base::cur, std::ios_base::in);
    if (current >= m_End) return traits_type::eof();
    return m_Source->sgetc();
}

std::streambuf::int_type PackageBuffer::uflow() {
    std::streampos current = m_Source->pubseekoff(0, std::ios_base::cur, std::ios_base::in);
    if (current >= m_End) return traits_type::eof();
    return m_Source->sbumpc();
}

std::streamsize PackageBuffer::xsgetn(char* s, std::streamsize n) {
    std::streampos current = m_Source->pubseekoff(0, std::ios_base::cur, std::ios_base::in);
    std::streamsize remaining = static_cast<std::streamsize>(m_End - current);
    if (remaining <= 0) return 0;
    std::streamsize toRead = std::min(n, remaining);
    return m_Source->sgetn(s, toRead);
}

std::streampos PackageBuffer::seekoff(std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode which) {
    std::streampos base;
    switch (way) {
        case std::ios_base::beg: base = m_Start; break;
        case std::ios_base::cur: base = m_Source->pubseekoff(0, std::ios_base::cur, which); break;
        case std::ios_base::end: base = m_End; break;
        default: return -1;
    }

    std::streampos newpos = base + off;
    if (newpos < m_Start || newpos > m_End) return -1;

    return m_Source->pubseekpos(newpos);
}

std::streampos PackageBuffer::seekpos(std::streampos pos, std::ios_base::openmode which) {
    if (pos < 0 || m_Start + pos > m_End) return -1;
    return m_Source->pubseekpos(m_Start + pos);
}

PackageStream::PackageStream(CREFSTR path, std::streampos start, std::streamsize size)
    : m_Stream(std::ifstream(path, std::ios::binary)),
      m_Buf(m_Stream.rdbuf(), start, size),
      std::istream(&m_Buf)
{
    if (!m_Stream) {
        setstate(std::ios::failbit);
    }
}

struct FileEntry
{
    fs::path path;
    std::string filename;
    uint64_t size;
    uint64_t offset;
};

px::AssetPackage::AssetPackage(CREFSTR path)
{
    m_Path = path;
    m_Stream.open(path, std::ios::binary);

    if (fs::file_size(path) <= 4)
    {
        Error::Throw(PX_ERROR_APKG_FILE, "Broken or corrupt file");
        return;
    }

    char buf[4];
    m_Stream.read(buf, sizeof(buf));
    if (buf[0] != 'A' || buf[1] != 'P' || buf[2] != 'K' || buf[3] != 'G')
    {
        Error::Throw(PX_ERROR_APKG_FILE, "Broken or corrupt file (invalid header)");
        return;
    }

    uint8_t ver;
    m_Stream.read(reinterpret_cast<char*>(&ver), sizeof(ver));
    if (ver != PX_APKG_VERSION)
    {
        Error::Throw(PX_ERROR_APKG_FILE, "Incompatible version");
        return;
    }

    uint32_t tableSize;
    m_Stream.read(reinterpret_cast<char*>(&tableSize), sizeof(tableSize));

    for (uint32_t i = 0; i < tableSize; i++)
    {
        uint16_t nameLen;
        m_Stream.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));

        char* name = new char[nameLen + 1];
        m_Stream.read(name, nameLen);
        name[nameLen] = 0;

        uint64_t offset;
        m_Stream.read(reinterpret_cast<char*>(&offset), sizeof(offset));

        m_LocationTable.insert({ name, offset });

        delete[] name;
    }
}

px::AssetPackage::~AssetPackage()
{
    if (m_Stream.is_open()) m_Stream.close();
}

std::unique_ptr<PackageStream> px::AssetPackage::OpenStream(CREFSTR path)
{
    if (m_LocationTable.count(path) < 1)
    {
        Error::Throw(PX_ERROR_APKG_FILE, "Packaged file not found");
        return nullptr;
    }

    uint64_t offset = m_LocationTable.at(path);
    m_Stream.seekg(offset);

    uint64_t size;
    m_Stream.read(reinterpret_cast<char*>(&size), sizeof(size));

    std::unique_ptr<PackageStream> stream = std::make_unique<PackageStream>(m_Path, offset + sizeof(uint64_t), size);
    if (!(*stream.get()))
    {
        Error::Throw(PX_ERROR_APKG_FILE, "Failed to open substream");
        return nullptr;
    }

    return std::move(stream);
}

void px::AssetPackage::SaveToFile(CREFSTR path, CREFSTR outPath)
{
    if (m_LocationTable.count(path) < 1)
    {
        Error::Throw(PX_ERROR_APKG_FILE, "Packaged file not found");
        return;
    }

    std::ofstream out(outPath);
    if (!out.is_open())
    {
        Error::Throw(PX_ERROR_APKG_FILE, "Couldn't open output file");
        return;
    }

    uint64_t offset = m_LocationTable.at(path);

    m_Stream.seekg(offset);

    uint64_t size;
    m_Stream.read(reinterpret_cast<char*>(&size), sizeof(size));

    for (uint64_t i = 0; i < size; i++)
    {
        char byte;
        m_Stream.read(&byte, sizeof(byte));
        out.write(&byte, sizeof(byte));
    }

    out.close();
}

bool px::AssetPackage::HasFile(CREFSTR path)
{
    return m_LocationTable.count(path) > 0;
}

static uint64_t GetFileSize(const fs::path &file)
{
    return fs::file_size(file);
}

static void RecurSearch(std::vector<fs::path>& vec, const fs::path& where)
{
    if (!fs::is_directory(where)) return;
    for (fs::directory_entry entry : fs::directory_iterator(where))
    {
        if (entry.is_directory())
        {
            RecurSearch(vec, entry);
        }
        else if (entry.is_regular_file())
        {
            vec.push_back(entry);
        }
    }
}

APKG px::AssetPackage::OpenPackage(CREFSTR path)
{
    if (!fs::exists(path))
    {
        Error::Throw(PX_ERROR_APKG_FILE, "Couldn't find directory to pack");
        return nullptr;
    }

    APKG package = new AssetPackage(path);
    if (Error::HasError())
    {
        delete package;
        return nullptr;
    }

    return package;
}

void px::AssetPackage:: ClosePackage(APKG package)
{
    if (!package) return;
    delete package;
}

Error px::AssetPackage::PackDirectory(CREFSTR path, CREFSTR outPath)
{
    if (!fs::exists(path))
    {
        Error::Throw(PX_ERROR_APKG_FILE, "Couldn't find directory to pack");
        return PX_ERROR_APKG_FILE;
    }

    std::vector<fs::path> files;
    std::vector<FileEntry> entries;
    RecurSearch(files, path);

    uint64_t totalOffset = 0;
    for (const fs::path& f : files)
    {
        FileEntry entry{};
        entry.path = f;
        entry.filename = fs::relative(f, path).string();
        entry.size = GetFileSize(f);
        entry.offset = totalOffset;
        totalOffset += 8 + entry.size;
        entries.push_back(entry);
    }

    std::ofstream o(outPath, std::ios::binary);
    if (!o.is_open())
    {
        Error::Throw(PX_ERROR_APKG_FILE, "Can't open output file");
        return PX_ERROR_APKG_FILE;
    }

    uint64_t locTableSize = 0;
    for (const FileEntry& entry : entries)
    {
        locTableSize += sizeof(uint16_t) + entry.filename.length() + sizeof(uint64_t);
    }
    
    o << "APKG";

    uint8_t ver = PX_APKG_VERSION;
    o.write(reinterpret_cast<char*>(&ver), sizeof(ver));

    uint32_t fileCount = entries.size();
    o.write(reinterpret_cast<char*>(&fileCount), sizeof(fileCount));

    uint64_t startOffset = o.tellp();
    startOffset += locTableSize;
    for (const FileEntry& entry : entries)
    {
        std::string filename = entry.filename;
        uint16_t len = filename.length();

        o.write(reinterpret_cast<char*>(&len), sizeof(len));
        o.write(filename.c_str(), len);

        uint64_t offset = startOffset + entry.offset;
        o.write(reinterpret_cast<const char*>(&offset), sizeof(offset));
    }

    for (const FileEntry& entry : entries)
    {
        o.write(reinterpret_cast<const char*>(&entry.size), sizeof(entry.size));

        std::ifstream in(entry.path, std::ios::binary);
        if (!in.is_open())
        {
            PX_DEBUG_LOG("AssetPackage::PackDirectory()", "Failed to open file: %s", entry.path);
            uint8_t zero = 0;
            for (uint64_t i = 0; i < entry.size; i++)
            {
                o.write(reinterpret_cast<char*>(&zero), sizeof(zero));
            }
            continue;
        }

        char buf[4096];
        while (in.read(buf, sizeof(buf)))
        {
            o.write(buf, in.gcount());
        }
        o.write(buf, in.gcount());

        in.close();
    }

    o.close();
    return PX_NOERROR;
}
