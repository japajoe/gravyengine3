#include "Stopwatch.hpp"

namespace GravyEngine
{
    Stopwatch::Stopwatch()
    {
        running = false;
    }

    void Stopwatch::Start() 
    {
        startTimePoint = std::chrono::high_resolution_clock::now();
        running = true;
    }

    void Stopwatch::Stop() 
    {
        if (running) {
            endTimePoint = std::chrono::high_resolution_clock::now();
            running = false;
        }
    }

    void Stopwatch::Reset() 
    {
        startTimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>();
        endTimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>();
        running = false;
    }

    double Stopwatch::GetElapsedMilliseconds() const 
    {
        return std::chrono::duration<double, std::milli>(GetElapsedTime()).count();
    }

    double Stopwatch::GetElapsedSeconds() const 
    {
        return std::chrono::duration<double>(GetElapsedTime()).count();
    }

    std::chrono::duration<double> Stopwatch::GetElapsedTime() const
    {
        if (running)
            return std::chrono::high_resolution_clock::now() - startTimePoint;
        return endTimePoint - startTimePoint;
    }
};