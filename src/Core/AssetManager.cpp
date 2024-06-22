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

    void AssetManager::LoadFromPackAsyncThread(const std::vector<AssetLoadInfo> &assets)
    {
        for(auto &asset : assets)
        {
            if(resourcePacks.count(asset.resourcePackName) > 0)
            {
                auto pack = resourcePacks[asset.resourcePackName].get();
                AssetInfo info;
                info.type = asset.type;
                info.name = asset.filePath;
                info.data = pack->GetFileData(asset.filePath);
                assetQueue.Enqueue(info);
            }
        }
    }

    void AssetManager::LoadFromFileAsyncThread(const std::vector<AssetLoadInfo> &assets)
    {
        for(auto &asset : assets)
        {
            AssetInfo info;
            info.type = asset.type;
            info.name = asset.filePath;
            if(File::Exists(asset.filePath))
                info.data = File::ReadAllBytes(asset.filePath);
            assetQueue.Enqueue(info);
        }
    }

    void AssetManager::LoadFromPackAsync(const std::vector<AssetLoadInfo> &assets)
    {
        std::thread workerThread(LoadFromPackAsyncThread, assets);
        workerThread.detach();
    }

    void AssetManager::LoadFromFileAsync(const std::vector<AssetLoadInfo> &assets)
    {
        std::thread workerThread(LoadFromFileAsyncThread, assets);
        workerThread.detach();
    }

    void AssetManager::LoadFromPackAsync(AssetType type, const std::string &resourcePackName, const std::string &filepath)
    {
        if(resourcePacks.count(resourcePackName) == 0)
            return;
        
        auto result = std::async(std::launch::async, &AssetManager::GetDataFromPackAsync, type, resourcePackName, filepath);
    }

    void AssetManager::LoadFromFileAsync(AssetType type, const std::string &filepath)
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
            //Do only 1 asset per frame or this might block the main thread for a while
            if(assetQueue.TryDequeue(info))
            {
                GameBehaviourManager::OnAssetLoadedAsync(info);
            }
        }
    }
};