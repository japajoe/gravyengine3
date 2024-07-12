#ifndef GRAVYENGINE_REVERBFILTER_HPP
#define GRAVYENGINE_REVERBFILTER_HPP

#include "AudioEffect.hpp"
#include "MVerb.hpp"
#include <vector>
#include <memory>

namespace GravyEngine
{
    class MVerbPreset
    {
    public:
        float dampingFrequency;
        float density;
        float bandwidthFrequency;
        float preDelayTime;
        float size;
        float decay;
        float gain;
        float mix;
        float earlyMix;
        MVerbPreset();
        static MVerbPreset CreateSubtle();
        static MVerbPreset CreateDark();
        static MVerbPreset CreateHalves();
        static MVerbPreset CreateCupboard();
        static MVerbPreset CreateStadium();
    };

    class ReverbFilter : public AudioEffect
    {
    public:
        ReverbFilter();
        void SetDampingFrequency(float value);
        float GetDampingFrequency() const;
        void SetDensity(float value);
        float GetDensity() const;
        void SetBandwidthFrequency(float value);
        float GetBandwidthFrequency() const;
        void SetPreDelayTime(float value);
        float GetPreDelayTime() const;
        void SetSize(float value);
        float GetSize() const;
        void SetDecay(float value);
        float GetDecay() const;
        void SetGain(float value);
        float GetGain() const;
        void SetMix(float value);
        float GetMix() const;
        void SetEarlyMix(float value);
        float GetEarlyMix() const;
        void OnProcess(AudioBuffer<float> pFrames, uint64_t frameCount, uint32_t channels) override;
        void LoadPreset(const MVerbPreset &preset);
    private:
        std::unique_ptr<MVerb<float>> reverb;
    };
};

#endif