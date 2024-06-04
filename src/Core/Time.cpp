#include "Time.hpp"

namespace GravyEngine
{
    Timer Time::tp1 = std::chrono::system_clock::now();
    Timer Time::tp2 = std::chrono::system_clock::now();
    float Time::deltaTime = 0;
    double Time::time = 0.0;
    int Time::FPS = 0;
    float Time::elapsedTime = 0;
    float Time::averageFPS = 0;

    void Time::Update()
    {
        tp2 = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsed = tp2 - tp1;
        tp1 = tp2;

        float dt = elapsed.count();
        
        deltaTime = dt;

        time += deltaTime;

        elapsedTime += dt;
        FPS++;

        if (elapsedTime > 0.5f)
        {
            averageFPS = FPS / elapsedTime;
            FPS = 0;
            elapsedTime = 0.0f;
        }
    }

    float Time::GetDeltaTime()
    {
        return deltaTime;
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
};