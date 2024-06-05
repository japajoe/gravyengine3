#ifndef GAMEBEHAVIOURMANAGER_HPP
#define GAMEBEHAVIOURMANAGER_HPP

#include "../External/imgui/ImGuiManager.h"
#include <vector>
#include <memory>

struct GLFWwindow;

namespace GravyEngine
{
    class GameBehaviour;

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
        static void OnRender();
        static void OnGUI();
    };
};
#endif