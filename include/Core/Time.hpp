#ifndef GRAVYENGINE_TIME_HPP
#define GRAVYENGINE_TIME_HPP

#include <chrono>
#include "../System/Diagnostics/Stopwatch.hpp"

namespace GravyEngine
{
    typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> Timer;

    struct TimerInfo
    {
        Timer tp1;
        Timer tp2;
        float deltaTime;
        TimerInfo()
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

    class Time
    {
    private:
        static TimerInfo timer;
        static TimerInfo timerFixed;
        static Stopwatch stopwatch;
        static double time;
        static int FPS;
        static float elapsedTime;
        static float averageFPS;
        static float frameTime;
    public:
        static float GetDeltaTime();
        static float GetFixedDeltaTime();
        static float GetFPS();
        static float GetTime();
        static double GetTimeAsDouble();
        static float GetFrameTime();
        static void OnStartFrame();
        static void OnEndFrame();
        static void OnFixedUpdate();
    };
};

#endif