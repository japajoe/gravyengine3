#ifndef GRAVYENGINE_ASSETMANAGER_HPP
#define GRAVYENGINE_ASSETMANAGER_HPP

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "../System/IO/ResourcePack.hpp"
#include "../System/Collections/ConcurrentQueue.hpp"

namespace GravyEngine
{
    struct AssetInfo
    {
        uint64_t id;
        std::string name;
        std::vector<uint8_t> data;
    };

    class AssetManager
    {
    friend class Application;
    public:
        static bool LoadResourcePack(const std::string &name, const std::string &filepath, const std::string &key);
        static void LoadFileAsync(uint64_t uniqueId, const std::string &resourcePackName, const std::string &filepath);
        static void LoadFileAsync(uint64_t uniqueId, const std::string &filepath);
    private:
        static std::unordered_map<std::string, std::unique_ptr<ResourcePack>> resourcePacks;
        static ConcurrentQueue<AssetInfo> assetQueue;
        static void GetDataFromPackAsync(uint64_t uniqueId, const std::string resourcePackName, const std::string &filepath);
        static void GetDataFromFileAsync(uint64_t uniqueId, const std::string &filepath);
        static void OnEndFrame();
    };
};

#endif