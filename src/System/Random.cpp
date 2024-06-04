#include "Random.hpp"
#include "Mathf.hpp"
#include <random>

namespace GravyEngine
{
    std::random_device rdev;
    std::mt19937 rgen(rdev());
    std::uniform_int_distribution<uint8_t> dist_byte(0, 255);
    std::uniform_real_distribution<double> dist_double(0.0, 1.0);

    void Random::Seed(uint32_t seed)
    {
        rgen.seed(seed);
    }

    double Random::GetNextDouble()
    {
        return dist_double(rgen);
    }

    uint8_t Random::GetNextByte()
    {
        return dist_byte(rgen);
    }

    double Random::Range(double min, double max)
    {
        double x = GetNextDouble() * (max - min);
        return min + x;
    }

    void Random::InsideUnitSphere(float &x, float &y, float &z)
    {
        float theta = static_cast<float>(2 * Mathf::PI * GetNextDouble());
        x = Mathf::Sin(theta);
        y = Mathf::Cos(theta);
        z = Mathf::Sin(theta) * Mathf::Sin(theta);
    }
};