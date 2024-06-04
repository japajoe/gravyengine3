#include "GameBehaviourManager.hpp"
#include "GameBehaviour.hpp"
#include "../External/glfw/glfw3.h"

namespace GravyEngine
{
    std::vector<GameBehaviour*> GameBehaviourManager::behaviours;
    std::unique_ptr<ImGuiManager> GameBehaviourManager::imgui = nullptr;

    void GameBehaviourManager::Initialize(GLFWwindow *window)
    {
        imgui =std::make_unique<ImGuiManager>(window);
        imgui->Initialize();
    }

    void GameBehaviourManager::Deinitialize()
    {
        imgui->Deinitialize();
    }

    void GameBehaviourManager::Add(GameBehaviour *behaviour)
    {
        for(size_t i = 0; i < behaviours.size(); i++)
        {
            if(behaviours[i] == behaviour)
                return;
        }

        behaviours.push_back(behaviour);
    }

    void GameBehaviourManager::Remove(GameBehaviour *behaviour)
    {
        bool found = false;
        size_t index = 0;

        for(size_t i = 0; i < behaviours.size(); i++)
        {
            if(behaviours[i] == behaviour)
            {
                index = i;
                found = true;
                break;
            }
        }

        if(found)
        {
            behaviours.erase(behaviours.begin() + index);
        }
    }

    void GameBehaviourManager::OnApplicationQuit()
    {
        for(size_t i = 0; i < behaviours.size(); i++)
        {
            behaviours[i]->OnApplicationQuit();
        }

        behaviours.clear();
    }

    void GameBehaviourManager::OnUpdate()
    {
        for(size_t i = 0; i < behaviours.size(); i++)
        {
            behaviours[i]->OnUpdate();
        }
    }

    void GameBehaviourManager::OnLateUpdate()
    {
        for(size_t i = 0; i < behaviours.size(); i++)
        {
            behaviours[i]->OnLateUpdate();
        }
    }

    void GameBehaviourManager::OnFixedUpdate()
    {
        for(size_t i = 0; i < behaviours.size(); i++)
        {
            behaviours[i]->OnFixedUpdate();
        }
    }

    void GameBehaviourManager::OnRender()
    {
        for(size_t i = 0; i < behaviours.size(); i++)
        {
            behaviours[i]->OnRender();
        }
    }

    void GameBehaviourManager::OnGUI()
    {
        imgui->BeginFrame();

        for(size_t i = 0; i < behaviours.size(); i++)
        {
            behaviours[i]->OnGUI();
        }

        imgui->EndFrame();
    }
};