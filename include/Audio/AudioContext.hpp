#ifndef MINIAUDIOEX_AUDIOCONTEXT_HPP
#define MINIAUDIOEX_AUDIOCONTEXT_HPP

#include "../External/miniaudioex/miniaudioex.h"
#include "AudioSource.hpp"
#include "AudioListener.hpp"
#include <string>
#include <vector>
#include <cstdint>

namespace GravyEngine
{
    class AudioContext
    {
    private:
        static ma_ex_context *context;
        static std::vector<AudioSource*> sources;
        static std::vector<AudioListener*> listeners;
        static int32_t sampleRate;
        static int32_t channels;
    public:
        static void Initialize(uint32_t sampleRate, uint32_t channels);
        static void Deinitialize();
        static void Update();
        static ma_ex_context *GetContext();
        static void Add(AudioSource *source);
        static void Add(AudioListener *listener);
        static void Remove(AudioSource *source);
        static void Remove(AudioListener *listener);
        static int32_t GetSampleRate();
        static int32_t GetChannels();
        static void SetMasterVolume(float volume);
        static float GetMasterVolume();
    };
};

#endif