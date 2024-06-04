#include "AudioContext.hpp"
#include "../Core/Transform.hpp"

namespace GravyEngine
{
    ma_ex_context *AudioContext::context = nullptr;
    std::vector<AudioSource*> AudioContext::sources;
    std::vector<AudioListener*> AudioContext::listeners;
    int32_t AudioContext::sampleRate = 44100;
    int32_t AudioContext::channels = 2;
    FrameTimer AudioContext::frameTimer;

    void AudioContext::Initialize(uint32_t sampleRate, uint32_t channels)
    {
        AudioContext::sampleRate = sampleRate;
        AudioContext::channels = channels;

        ma_ex_context_config config = ma_ex_context_config_init(sampleRate, channels, 1024, NULL);
        context = ma_ex_context_init(&config);
    }

    void AudioContext::Deinitialize()
    {
        if(context != nullptr)
        {
            for(size_t i = 0; i < sources.size(); i++)
                sources[i]->Destroy();

            sources.clear();

            for(size_t i = 0; i < listeners.size(); i++)
                listeners[i]->Destroy();

            listeners.clear();

            ma_ex_context_uninit(context);
            context = nullptr;
        }
    }

    void AudioContext::Update()
    {
        if(context == nullptr)
            return;

        frameTimer.Update();

        for(size_t i = 0; i < listeners.size(); i++)
        {
            auto handle = listeners[i]->GetHandle();

            auto position = listeners[i]->GetTransform()->GetPosition();
            auto direction = listeners[i]->GetTransform()->GetForward();
            auto velocity = listeners[i]->GetTransform()->GetVelocity();

            ma_ex_audio_listener_set_position(handle, position.x, position.y, position.z);
            ma_ex_audio_listener_set_direction(handle, direction.x, direction.y, direction.z);
            ma_ex_audio_listener_set_velocity(handle, velocity.x, velocity.y, velocity.z);
        }

        for(size_t i = 0; i < sources.size(); i++)
        {
            sources[i]->Update();

            if(!sources[i]->GetSpatial())
                continue;
            
            auto handle = sources[i]->GetHandle();
            auto position = sources[i]->GetTransform()->GetPosition();
            auto direction = sources[i]->GetTransform()->GetForward();
            auto velocity = sources[i]->GetTransform()->GetVelocity();

            ma_ex_audio_source_set_position(handle, position.x, position.y, position.z);
            ma_ex_audio_source_set_direction(handle, direction.x, direction.y, direction.z);
            ma_ex_audio_source_set_velocity(handle, velocity.x, velocity.y, velocity.z);
        }
    }

    ma_ex_context *AudioContext::GetContext()
    {
        return context;
    }

    void AudioContext::Add(AudioSource *source)
    {
        for(size_t i = 0; i < sources.size(); i++)
        {
            if(sources[i] == source)
                return;
        }

        sources.push_back(source);
    }

    void AudioContext::Add(AudioListener *listener)
    {
        for(size_t i = 0; i < listeners.size(); i++)
        {
            if(listeners[i] == listener)
                return;
        }

        listeners.push_back(listener);
    }

    void AudioContext::Remove(AudioSource *source)
    {
        bool found = false;
        size_t index = 0;

        for(size_t i = 0; i < sources.size(); i++)
        {
            if(sources[i] == source)
            {
                index = i;
                found = true;
                break;
            }
        }

        if(found)
        {
            sources[index]->Destroy();
            sources.erase(sources.begin() + index);
        }
    }

    void AudioContext::Remove(AudioListener *listener)
    {
        bool found = false;
        size_t index = 0;

        for(size_t i = 0; i < listeners.size(); i++)
        {
            if(listeners[i] == listener)
            {
                index = i;
                found = true;
                break;
            }
        }

        if(found)
        {
            listeners[index]->Destroy();
            listeners.erase(listeners.begin() + index);
        }
    }

    int32_t AudioContext::GetSampleRate()
    {
        return sampleRate;
    }

    int32_t AudioContext::GetChannels()
    {
        return channels;
    }

    float AudioContext::GetDeltaTime()
    {
        return frameTimer.deltaTime;
    }

    void AudioContext::SetMasterVolume(float volume)
    {
        if(context == nullptr)
            return;
        ma_ex_context_set_master_volume(context, volume);
    }

    float AudioContext::GetMasterVolume()
    {
        if(context == nullptr)
            return 0.0f;
        return ma_ex_context_get_master_volume(context);
    }
};