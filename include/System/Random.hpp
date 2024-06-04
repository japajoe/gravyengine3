#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <cstdint>

namespace GravyEngine
{
    class Random
    {
    public:
        static void Seed(uint32_t seed);
        static double GetNextDouble();
        static uint8_t GetNextByte();
        static double Range(double min, double max);
        static void InsideUnitSphere(float &x, float &y, float &z);
    };
};

#endif