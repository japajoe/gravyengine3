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

    void GameBehaviour::OnAssetLoadedAsync(uint64_t id, const std::string &name, const std::vector<uint8_t> &data)
    {

    }

    void GameBehaviour::LoadAssetFromResourceAsync(uint64_t id, const std::string &resourcePackName, const std::string &filepath)
    {
        AssetManager::LoadFileAsync(id, resourcePackName, filepath);
    }

    void GameBehaviour::LoadAssetFromFileAsync(uint64_t id, const std::string &resourcePackName, const std::string &filepath)
    {
        AssetManager::LoadFileAsync(id, filepath);
    }
};