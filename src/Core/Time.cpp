#include "Time.hpp"

namespace GravyEngine
{
    TimerInfo Time::timer;
    TimerInfo Time::timerFixed;
    Stopwatch Time::stopwatch;
    double Time::time = 0.0;
    int Time::FPS = 0;
    float Time::elapsedTime = 0.0f;
    float Time::averageFPS = 0.0f;
    float Time::frameTime = 0.0f;

    void Time::OnStartFrame()
    {
        stopwatch.Start();

        timer.Update();
        
        time += timer.deltaTime;

        elapsedTime += timer.deltaTime;
        FPS++;

        if (elapsedTime > 0.5f)
        {
            averageFPS = FPS / elapsedTime;
            FPS = 0;
            elapsedTime = 0.0f;
        }
    }

    void Time::OnEndFrame()
    {
        stopwatch.Stop();
        frameTime = stopwatch.GetElapsedMilliseconds();
    }

    void Time::OnFixedUpdate()
    {
        timerFixed.Update();
    }

    float Time::GetDeltaTime()
    {
        return timer.deltaTime;
    }

    float Time::GetFixedDeltaTime()
    {
        return timerFixed.deltaTime;
    }

    float Time::GetFPS()
    {
        return averageFPS;
    }

    float Time::GetTime()
    {
        return static_cast<float>(time);
    }

    double Time::GetTimeAsDouble()
    {
        return time;
    }

    float Time::GetFrameTime()
    {
        return frameTime;
    }
};