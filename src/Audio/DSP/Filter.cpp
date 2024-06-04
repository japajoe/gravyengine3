#include "Filter.hpp"
#include "../AudioContext.hpp"

namespace GravyEngine
{
    Filter::Filter()
    {
        ma_filter_config config = ma_filter_config_init(ma_filter_type_lowpass, AudioContext::GetSampleRate(), 440.0f, 0.5f, 0.0f);
        ma_filter_init(&config, &filter);
    }

    Filter::Filter(FilterType type, float frequency, float q, float gainDB, int sampleRate)
    {
        ma_filter_config config = ma_filter_config_init(static_cast<ma_filter_type>(type), sampleRate, frequency, q, gainDB);
        ma_filter_init(&config, &filter);
    }

    float Filter::Process(float input)
    {
        return ma_filter_process_sample(&filter, input);
    }

    void Filter::Process(AudioBuffer<float> framesOut, uint64_t frameCount, uint32_t channels)
    {
        uint64_t sampleCount = frameCount * channels;
        float *pFramesOut = framesOut.GetPointer<float>();
        ma_filter_process_samples(&filter, pFramesOut, sampleCount, pFramesOut, sampleCount);
    }

    void Filter::SetType(FilterType type)
    {
        ma_filter_set_type(&filter, static_cast<ma_filter_type>(type));
    }

    FilterType Filter::GetType() const
    {
        return static_cast<FilterType>(ma_filter_get_type(&filter));
    }

    void Filter::SetFrequency(float frequency)
    {
        ma_filter_set_frequency(&filter, frequency);
    }

    float Filter::GetFrequency() const
    {
        return ma_filter_get_frequency(&filter);
    }

    void Filter::SetQ(float q)
    {
        ma_filter_set_q(&filter, q);
    }

    float Filter::GetQ() const
    {
        return ma_filter_get_q(&filter);
    }

    void Filter::SetGainDB(float gainDB)
    {
        ma_filter_set_gain_db(&filter, gainDB);
    }

    float Filter::GetGainDB() const
    {
        return ma_filter_get_gain_db(&filter);
    }
};