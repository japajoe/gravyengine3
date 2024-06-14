#include "AssetManager.hpp"
#include "GameBehaviourManager.hpp"
#include "../System/IO/File.hpp"
#include <future>

namespace GravyEngine
{
    std::unordered_map<std::string, std::unique_ptr<ResourcePack>> AssetManager::resourcePacks;
    ConcurrentQueue<AssetInfo> AssetManager::assetQueue;
    
    bool AssetManager::LoadResourcePack(const std::string &name, const std::string &filepath, const std::string &key)
    {
        if(resourcePacks.count(name) > 0)
            return true;

        if(!File::Exists(filepath))
            return false;

        resourcePacks[name] = std::make_unique<ResourcePack>();
        auto pack = resourcePacks[name].get();

        return pack->LoadPack(filepath, key);
    }

    void AssetManager::LoadFileAsync(uint64_t uniqueId, const std::string &resourcePackName, const std::string &filepath)
    {
        if(resourcePacks.count(resourcePackName) == 0)
            return;
        
        auto result = std::async(std::launch::async, &AssetManager::GetDataFromPackAsync, uniqueId, resourcePackName, filepath);
    }

    void AssetManager::LoadFileAsync(uint64_t uniqueId, const std::string &filepath)
    {
        if(!File::Exists(filepath))
            return;
        
        auto result = std::async(std::launch::async, &AssetManager::GetDataFromFileAsync, uniqueId, filepath);
    }

    void AssetManager::GetDataFromPackAsync(uint64_t uniqueId, const std::string resourcePackName, const std::string &filepath)
    {
        auto pack = resourcePacks[resourcePackName].get();
        AssetInfo info;
        info.id = uniqueId;
        info.name = filepath;
        info.data = pack->GetFileData(filepath);
        assetQueue.Enqueue(info);
    }

    void AssetManager::GetDataFromFileAsync(uint64_t uniqueId, const std::string &filepath)
    {
        AssetInfo info;
        info.id = uniqueId;
        info.name = filepath;
        info.data = File::ReadAllBytes(filepath);
        assetQueue.Enqueue(info);
    }

    void AssetManager::OnEndFrame()
    {
        if(assetQueue.GetCount() > 0)
        {
            AssetInfo info;
            while(assetQueue.TryDequeue(info))
            {
                GameBehaviourManager::OnAssetLoadedAsync(info.id, info.name, info.data);
            }
        }
    }
};