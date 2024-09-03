#ifndef GRAVYENGINE_GAMEBEHAVIOURMANAGER_HPP
#define GRAVYENGINE_GAMEBEHAVIOURMANAGER_HPP

#include "AssetInfo.hpp"
#include "../External/imgui/ImGuiManager.h"
#include <vector>
#include <memory>
#include <cstdint>
#include <string>

struct GLFWwindow;

namespace GravyEngine
{
    class GameBehaviour;
    class Rigidbody;

    class GameBehaviourManager
    {
    private:
        static std::vector<GameBehaviour*> behaviours;
        static std::unique_ptr<ImGuiManager> imgui;
    public:
        static void Initialize(GLFWwindow *window);
        static void Deinitialize();
        static void Add(GameBehaviour *behaviour);
        static void Remove(GameBehaviour *behaviour);
        static void OnApplicationQuit();
        static void OnUpdate();
        static void OnLateUpdate();
        static void OnFixedUpdate();
        static void OnGUI();
        static void OnEndFrame();
        static void OnAssetLoadedAsync(const AssetInfo &assetInfo);
        static void OnCollisionEnter(Rigidbody *a, Rigidbody *b);
        static void OnCollisionStay(Rigidbody *a, Rigidbody *b);
        static void OnCollisionExit(Rigidbody *rigidBody);
    };
};
#endif