#ifndef GRAVYENGINE_NOISECALCULATOR_HPP
#define GRAVYENGINE_NOISECALCULATOR_HPP

#include "WaveCalculator.hpp"

namespace GravyEngine
{
    enum class NoiseType
    {
        Brown,
        Pink,
        White
    };

    class NoiseCalculator : public WaveCalculator
    {
    public:
        NoiseCalculator(NoiseType type);
        void SetType(NoiseType type);
        NoiseType GetType() const;
        float GetValue();
        float GetValue(float phase);
    private:
        NoiseType type;
        float (*noiseFunc)(NoiseCalculator*);
        float previousValue;
        void SetNoiseFunc();
        static float GetBrownNoise(NoiseCalculator *calculator);
        static float GetPinkNoise(NoiseCalculator *calculator);
        static float GetWhiteNoise(NoiseCalculator *calculator);
    };
};

#endif