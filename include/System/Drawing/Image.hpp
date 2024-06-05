#ifndef GRAVYENGINE_IMAGE_HPP
#define GRAVYENGINE_IMAGE_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include "Color.hpp"

namespace GravyEngine
{
    class Image
    {
    private:
        uint8_t *data;
        uint32_t width;
        uint32_t height;
        uint32_t channels;
        bool isLoaded;
    public:
        Image();
        Image(const std::string &filepath);
        Image(const uint8_t *compressedData, size_t size);
        Image(const uint8_t *uncompressedData, size_t size, uint32_t width, uint32_t height, uint32_t channels);
        Image(uint32_t width, uint32_t height, uint32_t channels, const Color &color);
        ~Image();
        uint8_t *GetData() const;
        uint32_t GetWidth() const;
        uint32_t GetHeight() const;
        uint32_t GetChannels() const;
        size_t GetDataSize() const;
        bool IsLoaded() const;
    private:
        bool LoadFromFile(const std::string &filepath);
        bool LoadFromMemory(const uint8_t *data, size_t size);
        bool Load(uint32_t width, uint32_t height, uint32_t channels, const Color &color);
    };
};

#endif
