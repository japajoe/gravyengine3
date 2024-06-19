#ifndef GRAVYENGINE_AUDIOEFFECT_HPP
#define GRAVYENGINE_AUDIOEFFECT_HPP

#include <cstdint>
#include "../AudioBuffer.hpp"

namespace GravyEngine
{
    class AudioEffect
    {
    public:
        virtual ~AudioEffect() {}
        virtual void OnProcess(AudioBuffer<float> pFrames, uint64_t frameCount, uint32_t channels) {}
        virtual void OnDestroy() {}
    };
};
#endif