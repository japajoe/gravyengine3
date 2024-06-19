#ifndef GRAVYENGINE_AUDIOGENERATOR_HPP
#define GRAVYENGINE_AUDIOGENERATOR_HPP

#include <cstdint>
#include "../AudioBuffer.hpp"

namespace GravyEngine
{
    class AudioGenerator
    {
    public:
        virtual ~AudioGenerator() {}
        virtual void OnGenerate(AudioBuffer<float> pFrames, uint64_t frameCount, uint32_t channels) {}
        virtual void OnDestroy() {}
    };
};

#endif