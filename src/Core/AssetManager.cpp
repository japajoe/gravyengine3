#include "AssetManager.hpp"
#include "GameBehaviourManager.hpp"
#include "../System/IO/File.hpp"
#include <future>
#include <iostream>

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

        bool status = pack->LoadPack(filepath, key);
        // auto &files = pack->GetFiles();

        // for(auto &file : files)
        // {
        //     std::cout << file.first << '\n';
        // }

        return status;
    }

    void AssetManager::LoadFileAsync(AssetType type, const std::string &resourcePackName, const std::string &filepath)
    {
        if(resourcePacks.count(resourcePackName) == 0)
            return;
        
        auto result = std::async(std::launch::async, &AssetManager::GetDataFromPackAsync, type, resourcePackName, filepath);
    }

    void AssetManager::LoadFileAsync(AssetType type, const std::string &filepath)
    {
        if(!File::Exists(filepath))
            return;
        
        auto result = std::async(std::launch::async, &AssetManager::GetDataFromFileAsync, type, filepath);
    }

    void AssetManager::GetDataFromPackAsync(AssetType type, const std::string resourcePackName, const std::string &filepath)
    {
        auto pack = resourcePacks[resourcePackName].get();
        AssetInfo info;
        info.type = type;
        info.name = filepath;
        info.data = pack->GetFileData(filepath);
        assetQueue.Enqueue(info);
    }

    void AssetManager::GetDataFromFileAsync(AssetType type, const std::string &filepath)
    {
        AssetInfo info;
        info.type = type;
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
                GameBehaviourManager::OnAssetLoadedAsync(info);
            }
        }
    }
};