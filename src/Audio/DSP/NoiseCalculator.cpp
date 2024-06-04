#include "NoiseCalculator.hpp"
#include "../../System/Random.hpp"

namespace GravyEngine
{
    NoiseCalculator::NoiseCalculator(NoiseType type)
    {
        this->type = type;
        previousValue = 0.0f;
        SetNoiseFunc();
    }

    void NoiseCalculator::SetType(NoiseType type)
    {
        this->type = type;
        previousValue = 0.0f;
        SetNoiseFunc();
    }

    NoiseType NoiseCalculator::GetType() const
    {
        return type;
    }

    float NoiseCalculator::GetValue()
    {
        return noiseFunc(this);
    }

    float NoiseCalculator::GetValue(float phase)
    {
        return noiseFunc(this);
    }

    void NoiseCalculator::SetNoiseFunc()
    {
        switch(type)
        {
            case NoiseType::Brown:
                noiseFunc = GetBrownNoise;
                break;
            case NoiseType::Pink:
                noiseFunc = GetPinkNoise;
                break;
            case NoiseType::White:
                noiseFunc = GetWhiteNoise;
                break;
        }
    }

    float NoiseCalculator::GetBrownNoise(NoiseCalculator *calculator)
    {
        float newValue = static_cast<float>(Random::GetNextDouble()) * 2.0f - 1.0f;
        float output = (calculator->previousValue + newValue) * 0.5f;
        calculator->previousValue = newValue;
        return output;
    }

    float NoiseCalculator::GetPinkNoise(NoiseCalculator *calculator)
    {
        float value1 = static_cast<float>(Random::GetNextDouble()) * 2.0f - 1.0f;
        float value2 = static_cast<float>(Random::GetNextDouble()) * 2.0f - 1.0f;
        float output = (value1 + value2) * 0.5f;
        return output;
    }

    float NoiseCalculator::GetWhiteNoise(NoiseCalculator *calculator)
    {
        float output = static_cast<float>(Random::GetNextDouble()) * 2.0f - 1.0f;
        return output;
    }
};