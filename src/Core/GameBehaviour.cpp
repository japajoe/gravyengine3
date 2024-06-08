#include "GameBehaviour.hpp"
#include "GameBehaviourManager.hpp"

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
};