#ifndef MINIAUDIOEX_WAVETABLE_HPP
#define MINIAUDIOEX_WAVETABLE_HPP

#include "WaveCalculator.hpp"
#include <cstdlib>
#include <cstdint>
#include <vector>

namespace GravyEngine
{
    class Wavetable
    {
    public:
        Wavetable(WaveCalculator *calculator, int32_t length);
        Wavetable(const std::vector<float> &data);
        float GetValue(float frequency, float sampleRate);
        float GetValueAtPhase(float phase);
    private:
        float Interpolate(float value1, float value2, float t);
        std::vector<float> data;
        int32_t length;
        int32_t index;
        float phase;
        float phaseIncrement;
    };
};

#endif