#include "Wavetable.hpp"
#include "../../System/Mathf.hpp"
#include <cmath>

namespace GravyEngine
{
    Wavetable::Wavetable(WaveCalculator *calculator, int32_t length)
    {
        this->data.resize(length);
        this->length = length;
        this->phase = 0;
        this->phaseIncrement = 0;

        float phaseIncrement = static_cast<float>(Mathf::TAU / length);

        for (size_t i = 0; i < length; i++)
        {
            data[i] = calculator->GetValue(i * phaseIncrement);
        }
    }

    Wavetable::Wavetable(const std::vector<float> &data)
    {
        this->data = data;
        this->length = data.size();
        this->phase = 0;
        this->phaseIncrement = 0;
    }

    float Wavetable::GetValue(float frequency, float sampleRate)
    {
        float phase = this->phase > 0.0f ? (this->phase / Mathf::TAU) : 0.0f;

        this->phaseIncrement = Mathf::TAU * frequency / sampleRate;
        this->phase += this->phaseIncrement;
        this->phase = fmodf(this->phase, Mathf::TAU);

        index = (int)(phase * length);
        float t = phase * length - index;

        int i1 = index % length;
        int i2 = (index+1) % length;

        if(i1 < 0 || i2 < 0)
            return 0.0f;

        float value1 = data[i1];
        float value2 = data[i2];
        return Interpolate(value1, value2, t);
    }

    float Wavetable::GetValueAtPhase(float phase)
    {
        phase = phase > 0.0f ? (phase / Mathf::TAU) : 0.0f;

        index = (int)(phase * length);
        float t = phase * length - index;

        int i1 = index % length;
        int i2 = (index+1) % length;

        if(i1 < 0 || i2 < 0)
            return 0.0f;

        float value1 = data[i1];
        float value2 = data[i2];
        return Interpolate(value1, value2, t);
    }

    float Wavetable::Interpolate(float value1, float value2, float t)
    {
        return value1 + (value2 - value1) * t;
    }
};