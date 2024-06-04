#ifndef MINIAUDIOEX_AUDIOLISTENER_HPP
#define MINIAUDIOEX_AUDIOLISTENER_HPP

#include "../External/miniaudioex/miniaudioex.h"
#include "../System/Numerics/Vector3.hpp"
#include "../Core/Component.hpp"

namespace GravyEngine
{
    class AudioListener : public Component
    {
    friend class AudioContext;
    public:
        AudioListener();    
        void SetIsEnabled(bool enabled);
        bool GetIsEnabled() const;
        void SetWorldUp(const Vector3 &worldUp);
        Vector3 GetWorldUp() const;
        ma_ex_audio_listener *GetHandle() const;
    protected:
        void OnInitialize() override;
        void OnDestroy() override;
    private:
        ma_ex_audio_listener *handle;
        Vector3 previousPosition;
        void Destroy();
    };
};

#endif