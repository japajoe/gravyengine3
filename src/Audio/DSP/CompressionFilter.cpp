#include "CompressionFilter.hpp"
#include "../../System/Mathf.hpp"

namespace GravyEngine
{
    CompressionFilterPreset::CompressionFilterPreset()
    {
        threshold = 0.8f;
        ratio = 0.2f;
        attack = 500.0f;
        release = 1000.0f;
    }
    
    CompressionFilterPreset::CompressionFilterPreset(float threshold, float ratio, float attack, float release)
    {
        this->threshold = threshold;
        this->ratio = ratio;
        this->attack = attack;
        this->release = release;
    }

    CompressionFilterPreset CompressionFilterPreset::Light()
    {
        return CompressionFilterPreset(0.8f, 0.2f, 500.0f, 1000.0f);
    }

    CompressionFilterPreset CompressionFilterPreset::Heavy()
    {
        return CompressionFilterPreset(0.5f, 0.8f, 200.0f, 500.0f);
    }

    CompressionFilterPreset CompressionFilterPreset::Smooth()
    {
        return CompressionFilterPreset(0.6f, 0.5f, 800.0f, 1200.0f);
    }

    CompressionFilterPreset CompressionFilterPreset::Punchy()
    {
        return CompressionFilterPreset(0.7f, 0.6f, 400.0f, 800.0f);
    }

    CompressionFilter::CompressionFilter() : AudioEffect()
    {
        threshold = 1.0f;
        attack = 0.0f;
        release = 0.0f;
        envelope_decay = 0.0f;
        output = 1.0f;
        transfer_A = 0.0f;
        transfer_B = 1.0f;
        env = 0.0f;
        gain = 1.0f;
        LoadPreset(CompressionFilterPreset::Smooth());
    }

    void CompressionFilter::SetThreshold(float threshold)
    {
        this->threshold = threshold;
        transfer_B = output * Mathf::Pow(threshold, -transfer_A);
    }

    float CompressionFilter::GetThreshold() const
    {
        return threshold;
    }

    void CompressionFilter::SetRatio(float ratio)
    {
        transfer_A = ratio - 1.0f;
        transfer_B = output * Mathf::Pow(threshold, -transfer_A);
    }

    float CompressionFilter::GetRatio() const
    {
        return transfer_A;
    }

    void CompressionFilter::SetAttack(float attack)
    {
        this->attack = Mathf::Exp(-1.0f / attack);
    }

    float CompressionFilter::GetAttack() const
    {
        return attack;
    }

    void CompressionFilter::SetRelease(float release)
    {
        this->release = Mathf::Exp(-1.0f / release);
        envelope_decay = Mathf::Exp(-4.0f / release); /* = Mathf.Exp(-1/(0.25*value)) */
    }

    float CompressionFilter::GetRelease()
    {
        return release;
    }

    void CompressionFilter::SetOutput(float output)
    {
        this->output = output;
        transfer_B = output * Mathf::Pow(threshold, -transfer_A);
    }

    float CompressionFilter::GetOutput() const
    {
        return output;
    }

    void CompressionFilter::Reset()
    {
        env = 0.0f; 
        gain = 1.0f;
    }

    void CompressionFilter::LoadPreset(const CompressionFilterPreset &preset)
    {
        Reset();
        SetAttack(preset.attack);
        SetRatio(preset.ratio);
        SetRelease(preset.release);
        SetThreshold(preset.threshold);
    }

    void CompressionFilter::OnProcess(AudioBuffer<float> pFrames, uint64_t frameCount, uint32_t channels)
    {
        float det = 0.0f;
        float transfer_gain = 0.0f;
        float inputLeft = 0.0f;
        float inputRight = 0.0f;

        int index = 0;

        for(size_t i = 0; i < frameCount; i++)
        {
            inputLeft = pFrames[index+0];
            inputRight = pFrames[index+1];

            det = Mathf::Max(Mathf::Abs(inputLeft), Mathf::Abs(inputRight));
            det += 10e-30f; /* add tiny DC offset (-600dB) to prevent denormals */

            env = det >= env ? det : det+envelope_decay*(env-det);

            transfer_gain = env > threshold ? (Mathf::Pow(env,transfer_A) * transfer_B) : output;

            gain = transfer_gain < gain ?
                                            transfer_gain+attack *(gain-transfer_gain):
                                            transfer_gain+release*(gain-transfer_gain);

            pFrames[index+0] = inputLeft * gain;
            pFrames[index+1] = inputRight * gain;

            index += channels;
        }
    }
};