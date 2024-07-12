#include "ReverbFilter.hpp"
#include "../../System/Mathf.hpp"
#include <cstring>

namespace GravyEngine
{
    MVerbPreset::MVerbPreset()
    {
        dampingFrequency = 0.0f;
        density = 0.5f;
        bandwidthFrequency = 1.0f;
        decay = 0.5f;
        preDelayTime = 0.0f;
        size = 0.5f;
        gain = 1.0f;
        mix = 0.15f;
        earlyMix = 0.75f;
    }
    
    MVerbPreset MVerbPreset::CreateSubtle()
    {
        MVerbPreset preset;
        //Subtle
        preset.dampingFrequency = 0.0f;
        preset.density = 0.5f;
        preset.bandwidthFrequency = 1.0f;
        preset.decay = 0.5f;
        preset.preDelayTime = 0.0f;
        preset.size = 0.5f;
        preset.gain = 1.0f;
        preset.mix = 0.15f;
        preset.earlyMix = 0.75f;
        return preset;
    }

    MVerbPreset MVerbPreset::CreateDark()
    {
        MVerbPreset preset;
        //Dark
        preset.dampingFrequency = 0.9f;
        preset.density = 0.5f;
        preset.bandwidthFrequency = 0.1f;
        preset.decay = 0.5f;
        preset.preDelayTime = 0.0f;
        preset.size = 0.5f;
        preset.gain = 1.0f;
        preset.mix = 0.5f;
        preset.earlyMix = 0.75f;
        return preset; 
    }

    MVerbPreset MVerbPreset::CreateHalves()
    {
        MVerbPreset preset;
        //Halves
        preset.dampingFrequency = 0.5f;
        preset.density = 0.5f;
        preset.bandwidthFrequency = 0.5f;
        preset.decay = 0.5f;
        preset.preDelayTime = 0.5f;
        preset.size = 0.75f;
        preset.gain = 1.0f;
        preset.mix = 0.5f;
        preset.earlyMix = 0.5f;
        return preset;
    }

    MVerbPreset MVerbPreset::CreateCupboard()
    {
        MVerbPreset preset;
        //Cupboard
        preset.dampingFrequency = 0.0f;
        preset.density = 0.5f;
        preset.bandwidthFrequency = 1.0f;
        preset.decay = 0.5f;
        preset.preDelayTime = 0.0f;
        preset.size = 0.25f;
        preset.gain = 1.0f;
        preset.mix = 0.35f;
        preset.earlyMix = 0.75f;
        return preset;
    }

    MVerbPreset MVerbPreset::CreateStadium()
    {
        MVerbPreset preset;
        //Stadium
        preset.dampingFrequency = 0.0f;
        preset.density = 0.5f;
        preset.bandwidthFrequency = 1.0f;
        preset.decay = 0.5f;
        preset.preDelayTime = 0.0f;
        preset.size = 1.0f;
        preset.gain = 1.0f;
        preset.mix = 0.35f;
        preset.earlyMix = 0.75f;
        return preset;
    }

    ReverbFilter::ReverbFilter() : AudioEffect()
    {
        reverb = std::make_unique<MVerb<float>>();
    }

    void ReverbFilter::SetDampingFrequency(float value)
    {
        reverb->setDampingFrequency(value);
    }

    float ReverbFilter::GetDampingFrequency() const
    {
        return reverb->getDampingFrequency();
    }

    void ReverbFilter::SetDensity(float value)
    {
        reverb->setDensity(value);
    }

    float ReverbFilter::GetDensity() const
    {
        return reverb->getDensity();
    }

    void ReverbFilter::SetBandwidthFrequency(float value)
    {
        reverb->setBandwidthFrequency(value);
    }

    float ReverbFilter::GetBandwidthFrequency() const
    {
        return reverb->getBandwidthFrequency();
    }

    void ReverbFilter::SetPreDelayTime(float value)
    {
        reverb->setPreDelayTime(value);
    }

    float ReverbFilter::GetPreDelayTime() const
    {
        return reverb->getPreDelayTime();
    }

    void ReverbFilter::SetSize(float value)
    {
        reverb->setSize(value);
    }

    float ReverbFilter::GetSize() const
    {
        return reverb->getSize();
    }

    void ReverbFilter::SetDecay(float value)
    {
        reverb->setDecay(value);
    }

    float ReverbFilter::GetDecay() const
    {
        return reverb->getDecay();
    }

    void ReverbFilter::SetGain(float value)
    {
        reverb->setGain(value);
    }

    float ReverbFilter::GetGain() const
    {
        return reverb->getGain();
    }

    void ReverbFilter::SetMix(float value)
    {
        reverb->setMix(value);
    }

    float ReverbFilter::GetMix() const
    {
        return reverb->getMix();
    }

    void ReverbFilter::SetEarlyMix(float value)
    {
        reverb->setEarlyMix(value);
    }

    float ReverbFilter::GetEarlyMix() const
    {
        return reverb->getEarlyMix();
    }

    void ReverbFilter::OnProcess(AudioBuffer<float> pFrames, uint64_t frameCount, uint32_t channels)
    {
        float *pInput = pFrames.GetPointer<float>();
        reverb->process(pInput, pInput, frameCount, channels);
    }

    void ReverbFilter::LoadPreset(const MVerbPreset &preset)
    {
        reverb->setDampingFrequency(preset.dampingFrequency);
        reverb->setDensity(preset.density);
        reverb->setBandwidthFrequency(preset.bandwidthFrequency);
        reverb->setPreDelayTime(preset.preDelayTime);
        reverb->setSize(preset.size);
        reverb->setDecay(preset.decay);
        reverb->setGain(preset.gain);
        reverb->setMix(preset.mix);
        reverb->setEarlyMix(preset.earlyMix);
    }
};