#ifndef MINIAUDIOEX_FILTER_HPP
#define MINIAUDIOEX_FILTER_HPP

#include <cstdint>
#include "../../External/miniaudioex/miniaudioex.h"
#include "../AudioBuffer.hpp"

namespace GravyEngine
{
    enum class FilterType
    {
        Lowpass,
        Highpass,
        Bandpass,
        Lowshelf,
        Highshelf,
        Peak,
        Notch
    };

    class Filter
    {
    public:
        Filter();
        Filter(FilterType type, float frequency, float q, float gainDB, int sampleRate = 0);
        float Process(float input);
        void Process(AudioBuffer<float> framesOut, uint64_t frameCount, uint32_t channels);
        void SetType(FilterType type);
        FilterType GetType() const;
        void SetFrequency(float frequency);
        float GetFrequency() const;
        void SetQ(float q);
        float GetQ() const;
        void SetGainDB(float gainDB);
        float GetGainDB() const;
    private:
        ma_biquad_filter filter;
    };
};
#endif