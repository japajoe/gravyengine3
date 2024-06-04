#include "FilterEffect.hpp"
#include "../AudioContext.hpp"

namespace GravyEngine
{
    FilterEffect::FilterEffect(FilterType type, float frequency, float q, float gainDB)
    {
        filter = Filter(type, frequency, q, gainDB, AudioContext::GetSampleRate());
    }

    void FilterEffect::SetType(FilterType type)
    {
        filter.SetType(type);
    }

    FilterType FilterEffect::GetType() const
    {
        return filter.GetType();
    }

    void FilterEffect::SetFrequency(float frequency)
    {
        filter.SetFrequency(frequency);
    }

    float FilterEffect::GetFrequency() const
    {
        return filter.GetFrequency();
    }

    void FilterEffect::SetQ(float q)
    {
        filter.SetQ(q);
    }

    float FilterEffect::GetQ() const
    {
        return filter.GetQ();
    }

    void FilterEffect::SetGainDB(float gainDB)
    {
        filter.SetGainDB(gainDB);
    }

    float FilterEffect::GetGainDB() const
    {
        return filter.GetGainDB();
    }

    void FilterEffect::OnProcess(AudioBuffer<float> pFrames, uint64_t frameCount, uint32_t channels)
    {
        filter.Process(pFrames, frameCount, channels);
    }
};