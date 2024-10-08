#ifndef GRAVYENGINE_GAMEBEHAVIOUR_HPP
#define GRAVYENGINE_GAMEBEHAVIOUR_HPP

#include "Component.hpp"
#include "AssetInfo.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace GravyEngine
{
    struct Collision;
    
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
        virtual void OnEndFrame();
        virtual void OnAssetLoadedAsync(const AssetInfo &assetInfo);
        virtual void OnCollisionEnter(const Collision *collision);
        virtual void OnCollisionStay(const Collision *collision);
        virtual void OnCollisionExit(const Collision *collision);
        void LoadAssetsFromResourceAsync(const std::vector<AssetLoadInfo> &assets);
        void LoadAssetsFromFileAsync(const std::vector<AssetLoadInfo> &assets);
        void LoadAssetFromResourceAsync(AssetType type, const std::string &resourcePackName, const std::string &filepath);
        void LoadAssetFromFileAsync(AssetType type, const std::string &filepath);
    };
};
#endif