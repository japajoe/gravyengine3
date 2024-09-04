#include "GameBehaviourManager.hpp"
#include "GameBehaviour.hpp"
#include "GameObject.hpp"
#include "../Physics/Rigidbody.hpp"
#include "../External/GLFW/glfw3.h"

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

        GameObject::DestroyAll();
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

    void GameBehaviourManager::OnGUI()
    {
        imgui->BeginFrame();

        for(size_t i = 0; i < behaviours.size(); i++)
        {
            behaviours[i]->OnGUI();
        }

        imgui->EndFrame();
    }

    void GameBehaviourManager::OnEndFrame()
    {
        GameObject::OnEndFrame();
    }

    void GameBehaviourManager::OnAssetLoadedAsync(const AssetInfo &assetInfo)
    {
        for(size_t i = 0; i < behaviours.size(); i++)
        {
            behaviours[i]->OnAssetLoadedAsync(assetInfo);
        }
    }

    void GameBehaviourManager::OnCollisionEnter(const Collision *collision)
    {
        #ifdef GRAVY_ENABLE_BULLET

        for(size_t i = 0; i < behaviours.size(); i++)
        {
            if(collision->body1->GetTransform()->GetRoot() == behaviours[i]->GetTransform()->GetRoot())
            {
                behaviours[i]->OnCollisionEnter(collision);
            }
        }

        #endif
    }

    void GameBehaviourManager::OnCollisionStay(const Collision *collision)
    {
        #ifdef GRAVY_ENABLE_BULLET

        for(size_t i = 0; i < behaviours.size(); i++)
        {
            if(collision->body1->GetTransform()->GetRoot() == behaviours[i]->GetTransform()->GetRoot())
            {
                behaviours[i]->OnCollisionStay(collision);
            }
        }

        #endif
    }

    void GameBehaviourManager::OnCollisionExit(const Collision *collision)
    {
        #ifdef GRAVY_ENABLE_BULLET

        for(size_t i = 0; i < behaviours.size(); i++)
        {
            if(collision->body1->GetTransform()->GetRoot() == behaviours[i]->GetTransform()->GetRoot())
            {
                behaviours[i]->OnCollisionExit(collision);
            }
        }

        #endif
    }
};