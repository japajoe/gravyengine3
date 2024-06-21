#ifndef GRAVYENGINE_ASSETINFO_HPP
#define GRAVYENGINE_ASSETINFO_HPP

#include <vector>
#include <cstdint>
#include <string>

namespace GravyEngine
{
    enum class AssetType
    {
        Audio,
        Blob,
        Model,
        Texture
    };

    struct AssetInfo
    {
        AssetType type;
        std::string name;
        std::vector<uint8_t> data;
    };

    struct AssetLoadInfo
    {
        AssetType type;
        std::string resourcePackName;
        std::string filePath;
        AssetLoadInfo(AssetType type, const std::string &resourcePackName, const std::string &filePath)
        {
            this->type = type;
            this->resourcePackName = resourcePackName;
            this->filePath = filePath;
        }
    };
};

#endif