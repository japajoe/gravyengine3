#ifndef GRAVYENGINE_STOPWATCH_HPP
#define GRAVYENGINE_STOPWATCH_HPP

#include <chrono>

namespace GravyEngine
{
    class Stopwatch 
    {
    public:
        Stopwatch();
        void Start();
        void Stop();
        void Reset();
        double GetElapsedMilliseconds() const;
        double GetElapsedSeconds() const;
        std::chrono::duration<double> GetElapsedTime() const;
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;
        std::chrono::time_point<std::chrono::high_resolution_clock> endTimePoint;
        bool running;
    };
};

#endif