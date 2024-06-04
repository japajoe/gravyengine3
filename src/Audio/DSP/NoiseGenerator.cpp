#include "NoiseGenerator.hpp"
#include "NoiseCalculator.hpp"
#include <vector>

namespace GravyEngine
{
    NoiseGenerator::NoiseGenerator()
    {
        NoiseCalculator calculator(NoiseType::Brown);
        wavetable = std::make_unique<Wavetable>(&calculator, 1024);
        frequency = 0.5f;
    }

    void NoiseGenerator::SetFrequency(float frequency)
    {
        this->frequency = frequency;
    }

    float NoiseGenerator::GetFrequency() const
    {
        return frequency;
    }

    void NoiseGenerator::OnGenerate(AudioBuffer<float> pFrames, uint64_t frameCount, uint32_t channels)
    {
        uint64_t frames = frameCount * channels;
        float sample = 0.0f;
        
        for(uint64_t i = 0; i < pFrames.GetLength(); i += channels)
        {
            sample = wavetable->GetValue(frequency, 48000);

            for(uint64_t j = 0; j < channels; j++)
            {
                pFrames[i+j] = sample;
            }
        }
    }
};