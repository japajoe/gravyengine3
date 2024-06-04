#ifndef MINIAUDIOEX_AUDIOCONTEXT_HPP
#define MINIAUDIOEX_AUDIOCONTEXT_HPP

#include "../External/miniaudioex/miniaudioex.h"
#include "AudioSource.hpp"
#include "AudioListener.hpp"
#include <string>
#include <vector>
#include <cstdint>
#include <chrono>

namespace GravyEngine
{
    typedef std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::nanoseconds> TimePoint;

    struct FrameTimer
    {
        TimePoint tp1;
        TimePoint tp2;
        float deltaTime;
        FrameTimer()
        {
            tp1 = std::chrono::system_clock::now();
            tp1 = std::chrono::system_clock::now();
            deltaTime = 0;
        }

        void Update()
        {
            tp2 = std::chrono::system_clock::now();
            std::chrono::duration<float> elapsed = tp2 - tp1;
            tp1 = tp2;
            deltaTime = elapsed.count();
        }
    };

    class AudioContext
    {
    private:
        static ma_ex_context *context;
        static std::vector<AudioSource*> sources;
        static std::vector<AudioListener*> listeners;
        static int32_t sampleRate;
        static int32_t channels;
        static FrameTimer frameTimer;
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
        static float GetDeltaTime();
        static void SetMasterVolume(float volume);
        static float GetMasterVolume();
    };
};

#endif