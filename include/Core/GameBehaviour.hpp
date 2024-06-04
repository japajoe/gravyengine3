#ifndef GAMEBEHAVIOUR_HPP
#define GAMEBEHAVIOUR_HPP

#include "Component.hpp"

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
        virtual void OnRender();
        virtual void OnGUI();
    };
};
#endif