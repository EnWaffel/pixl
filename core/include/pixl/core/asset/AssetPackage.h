#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"

#include <iostream>
#include <streambuf>
#include <fstream>
#include <unordered_map>
#include <istream>
#include <algorithm>
#include <memory>
#include <vector>

#define PX_APKG_VERSION 1

/*
    Asset Package File Format (apkg):

 	Offset | Size | Datatype | Value | Description
 
 	Header:
 
 	0	   | 4	  | int8	 | APKG  | apkg file declaration
    4      | 1    | uint8    | 1     | apkg format version
 	5      | 4    | uint32   | ?     | amount of packed files

    Location Table Entry:

    0      | 2    | uint16   | ?     | entry name length
    2      | ?    | char     | ?     | entry name
    + ?    | 8    | uint64   | ?     | entry offset
 
 	File:
 
 	? + ?  | 8    | uint64   | ?     | file size
 	?+?+8  | ?    | byte     | ?     | file data
*/

#define PX_ERROR_APKG_FILE 0x80

namespace px
{
    /**
     * @brief A buffer for a limited region inside a file.
     */
    class PackageBuffer : public std::streambuf
    {
    public:
        PX_INTERNAL PackageBuffer(std::streambuf* src, std::streampos startPos, std::streamsize size);
    protected:
        PX_INTERNAL int_type underflow() override;
        PX_INTERNAL int_type uflow() override;
        PX_INTERNAL std::streamsize xsgetn(char* s, std::streamsize n) override;
        PX_INTERNAL std::streampos seekoff(std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode which = std::ios_base::in) override;
        PX_INTERNAL std::streampos seekpos(std::streampos pos, std::ios_base::openmode which = std::ios_base::in) override;
    private:
        std::streambuf* m_Source;
        std::streampos m_Start;
        std::streampos m_End;
    };

    /**
     * An input stream from an Asset Package.
     */
    class PackageStream : public std::istream
    {
    public:
        PX_INTERNAL PackageStream(CREFSTR path, std::streampos start, std::streamsize size);
    private:
        std::ifstream m_Stream;
        PackageBuffer m_Buf;
    };

    class AssetPackage;
    typedef AssetPackage* APKG;

    /**
     * @brief An Asset Package is basically a zip file without the compression and the zip standard.
     */
    class AssetPackage
    {
    public:
        PX_INTERNAL AssetPackage(CREFSTR path);
        PX_INTERNAL ~AssetPackage();

        PX_API std::unique_ptr<PackageStream> OpenStream(CREFSTR path);
        PX_API void SaveToFile(CREFSTR path, CREFSTR outPath);
        PX_API bool HasFile(CREFSTR path);
        PX_API const std::unordered_map<std::string, uint64_t>& GetLocationTable();
        PX_API std::vector<std::string> GetFilesInDirectory(CREFSTR dir);
    private:
        std::string m_Path;
        std::ifstream m_Stream;
        std::unordered_map<std::string, uint64_t> m_LocationTable;
    public:
        /**
         * @brief Opens a package from a file path.
         * 
         * @param path The path to the package.
         */
        PX_API static APKG OpenPackage(CREFSTR path);
        /**
         * @brief Closes an Asset Package.
         * 
         * @param package The package to close
         */
        PX_API static void ClosePackage(APKG package);
        /**
         * @brief Packs an entire directory recursively into a .apkg file.
         * 
         * @param path The path of the directory to pack.
         * @param outPath The path of the output file.
         *
         * @return Returns PX_SUCCESS or an error code.
         */
        PX_API static Error PackDirectory(CREFSTR path, CREFSTR outPath);
    };
}