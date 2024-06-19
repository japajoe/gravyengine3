#ifndef GRAVYENGINE_TIME_HPP
#define GRAVYENGINE_TIME_HPP

#include <chrono>

namespace GravyEngine
{
    typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> Timer;

    class Time
    {
    private:
        static Timer tp1;
        static Timer tp2;
        static float deltaTime;
        static double time;
        static int FPS;
        static float elapsedTime;
        static float averageFPS;
    public:
        static float GetDeltaTime();
        static float GetFPS();
        static float GetTime();
        static double GetTimeAsDouble();
        static void Update();
    };
};

#endif