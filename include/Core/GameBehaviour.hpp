#ifndef GAMEBEHAVIOUR_HPP
#define GAMEBEHAVIOUR_HPP

#include "Component.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace GravyEngine
{
    class GameBehaviour : public Component
    {
    friend class GameBehaviourManager;
    public:
        GameBehaviour();
        virtual ~GameBehaviour();
    protected:
        virtual void OnApplicationQuit();
        virtual void OnUpdate();
        virtual void OnLateUpdate();
        virtual void OnFixedUpdate();
        virtual void OnGUI();
        virtual void OnAssetLoadedAsync(uint64_t id, const std::string &name, const std::vector<uint8_t> &data);
        void LoadAssetFromResourceAsync(uint64_t id, const std::string &resourcePackName, const std::string &filepath);
        void LoadAssetFromFileAsync(uint64_t id, const std::string &resourcePackName, const std::string &filepath);
    };
};
#endif