#include "GameBehaviour.hpp"
#include "GameBehaviourManager.hpp"
#include "AssetManager.hpp"

namespace GravyEngine
{
    GameBehaviour::GameBehaviour() : Component()
    {
        GameBehaviourManager::Add(this);
    }

    GameBehaviour::~GameBehaviour()
    {
        GameBehaviourManager::Remove(this);
    }

    void GameBehaviour::OnApplicationQuit()
    {

    }

    void GameBehaviour::OnUpdate()
    {

    }

    void GameBehaviour::OnLateUpdate()
    {

    }

    void GameBehaviour::OnFixedUpdate()
    {

    }

    void GameBehaviour::OnGUI()
    {

    }

    void GameBehaviour::OnAssetLoadedAsync(const AssetInfo &assetInfo)
    {

    }

    void GameBehaviour::OnCollisionEnter(const Collision *collision)
    {

    }

    void GameBehaviour::OnCollisionStay(const Collision *collision)
    {

    }

    void GameBehaviour::OnCollisionExit(const Collision *collision)
    {

    }

    void GameBehaviour::LoadAssetsFromResourceAsync(const std::vector<AssetLoadInfo> &assets)
    {
        AssetManager::LoadFromPackAsync(assets);
    }

    void GameBehaviour::LoadAssetsFromFileAsync(const std::vector<AssetLoadInfo> &assets)
    {
        AssetManager::LoadFromFileAsync(assets);
    }

    void GameBehaviour::LoadAssetFromResourceAsync(AssetType type, const std::string &resourcePackName, const std::string &filepath)
    {
        AssetManager::LoadFromPackAsync(type, resourcePackName, filepath);
    }

    void GameBehaviour::LoadAssetFromFileAsync(AssetType type, const std::string &filepath)
    {
        AssetManager::LoadFromFileAsync(type, filepath);
    }
};