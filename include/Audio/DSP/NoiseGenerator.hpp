#ifndef GRAVYENGINE_NOISEGENERATOR_HPP
#define GRAVYENGINE_NOISEGENERATOR_HPP

#include "AudioGenerator.hpp"
#include "Wavetable.hpp"
#include <memory>

namespace GravyEngine
{
    class NoiseGenerator : public AudioGenerator
    {
    private:
        std::unique_ptr<Wavetable> wavetable; 
        float frequency;
    public:
        NoiseGenerator();
        void SetFrequency(float frequency);
        float GetFrequency() const;
        void OnGenerate(AudioBuffer<float> pFrames, uint64_t frameCount, uint32_t channels) override;
    };
};

#endif