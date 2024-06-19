#ifndef GRAVYENGINE_ASSETMANAGER_HPP
#define GRAVYENGINE_ASSETMANAGER_HPP

#include "AssetInfo.hpp"
#include "../System/IO/ResourcePack.hpp"
#include "../System/Collections/ConcurrentQueue.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace GravyEngine
{
    class AssetManager
    {
    friend class Application;
    public:
        static bool LoadResourcePack(const std::string &name, const std::string &filepath, const std::string &key);
        static void LoadFileAsync(AssetType type, const std::string &resourcePackName, const std::string &filepath);
        static void LoadFileAsync(AssetType type, const std::string &filepath);
    private:
        static std::unordered_map<std::string, std::unique_ptr<ResourcePack>> resourcePacks;
        static ConcurrentQueue<AssetInfo> assetQueue;
        static void GetDataFromPackAsync(AssetType type, const std::string resourcePackName, const std::string &filepath);
        static void GetDataFromFileAsync(AssetType type, const std::string &filepath);
        static void OnEndFrame();
    };
};

#endif