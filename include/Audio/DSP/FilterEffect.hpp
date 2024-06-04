#ifndef MINIAUDIOEX_FILTEREFFECT_HPP
#define MINIAUDIOEX_FILTEREFFECT_HPP

#include "AudioEffect.hpp"
#include "Filter.hpp"

namespace GravyEngine
{
    class FilterEffect : public AudioEffect
    {
    public:
        FilterEffect(FilterType type, float frequency, float q, float gainDB);
        void SetType(FilterType type);
        FilterType GetType() const;
        void SetFrequency(float frequency);
        float GetFrequency() const;
        void SetQ(float q);
        float GetQ() const;
        void SetGainDB(float gainDB);
        float GetGainDB() const;
        void OnProcess(AudioBuffer<float> pFrames, uint64_t frameCount, uint32_t channels) override;
    private:
        Filter filter;
    };
};

#endif