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

    void GameBehaviour::LoadAssetFromResourceAsync(AssetType type, const std::string &resourcePackName, const std::string &filepath)
    {
        AssetManager::LoadFileAsync(type, resourcePackName, filepath);
    }

    void GameBehaviour::LoadAssetFromFileAsync(AssetType type, const std::string &resourcePackName, const std::string &filepath)
    {
        AssetManager::LoadFileAsync(type, filepath);
    }
};