#ifndef MINIAUDIOEX_AUDIOCLIP_HPP
#define MINIAUDIOEX_AUDIOCLIP_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace GravyEngine
{
    class AudioClip
    {
    public:
        AudioClip(const std::string &filepath, bool streamFromDisk = true);
        AudioClip(const std::vector<uint8_t> &data);
        AudioClip(void *data, size_t size);
        std::string GetFilePath() const;
        void *GetHandle() const;
        size_t GetDataSize() const;
        bool GetStreamFromDisk() const;
    private:
        std::string filePath;
        std::vector<uint8_t> data;
        size_t dataSize;
        void *handle;
        bool streamFromDisk;
    };
};

#endif