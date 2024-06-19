#ifndef GAMEBEHAVIOUR_HPP
#define GAMEBEHAVIOUR_HPP

#include "Component.hpp"
#include "AssetInfo.hpp"
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
        virtual void OnAssetLoadedAsync(const AssetInfo &assetInfo);
        void LoadAssetFromResourceAsync(AssetType type, const std::string &resourcePackName, const std::string &filepath);
        void LoadAssetFromFileAsync(AssetType type, const std::string &resourcePackName, const std::string &filepath);
    };
};
#endif