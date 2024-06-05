#ifndef GRAVYENGINE_RESOURCEPACK_HPP
#define GRAVYENGINE_RESOURCEPACK_HPP

#include <vector>
#include <string>
#include <map>
#include <cstdint>
#include <fstream>

namespace GravyEngine
{
    struct ResourceBuffer : public std::streambuf
    {
        ResourceBuffer(std::ifstream &ifs, uint32_t offset, uint32_t size);
        std::vector<uint8_t> vMemory;
    };

    struct ResourceFile
    {
        uint32_t nSize;
        uint32_t nOffset;
        std::string sFileName;
    };

    class ResourcePack : public std::streambuf
    {
    public:
        ResourcePack();
        ~ResourcePack();
        bool AddFile(const std::string &sFile);
        bool AddFile(const std::string &sFile, const std::string &sFileName);
        bool LoadPack(const std::string &sFile, const std::string &sKey);
        bool SavePack(const std::string &sFile, const std::string &sKey);
        ResourceBuffer GetFileBuffer(const std::string &sFile);
        std::vector<uint8_t> GetFileData(const std::string &sFile);
        bool Loaded() const;
        std::map<std::string, ResourceFile> GetFiles() const;
    private:
        std::map<std::string, ResourceFile> mapFiles;
        std::ifstream baseFile;
        std::vector<char> Scramble(const std::vector<char> &data, const std::string &key);
        std::string MakePosix(const std::string &path);
        uint16_t Checksum(const unsigned char* buf, uint16_t length);
    };
};

#endif