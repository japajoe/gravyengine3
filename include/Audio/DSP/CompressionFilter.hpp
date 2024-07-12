#ifndef GRAVYENGINE_COMPRESSIONFILTER_HPP
#define GRAVYENGINE_COMPRESSIONFILTER_HPP

#include "AudioEffect.hpp"

namespace GravyEngine
{
    class CompressionFilterPreset
    {
    public:
        float threshold;
        float ratio;
        float attack;
        float release;
        CompressionFilterPreset();
        CompressionFilterPreset(float threshold, float ratio, float attack, float release);
        static CompressionFilterPreset Light();
        static CompressionFilterPreset Heavy();
        static CompressionFilterPreset Smooth();
        static CompressionFilterPreset Punchy();
    };

    class CompressionFilter : public AudioEffect
    {
    private:
        float threshold;
        float attack;
        float release;
        float envelope_decay;
        float output;
        float transfer_A;
        float transfer_B;
        float env;
        float gain;
    public:
        CompressionFilter();
        void SetThreshold(float threshold);
        float GetThreshold() const;
        void SetRatio(float ratio);
        float GetRatio() const;
        void SetAttack(float attack);
        float GetAttack() const;
        void SetRelease(float release);
        float GetRelease();
        void SetOutput(float output);
        float GetOutput() const;
        void Reset();
        void LoadPreset(const CompressionFilterPreset &preset);
        void OnProcess(AudioBuffer<float> pFrames, uint64_t frameCount, uint32_t channels) override;
    };
};

#endif