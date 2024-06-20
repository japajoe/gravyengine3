#include "Mathf.hpp"
#include "Numerics/Noise.hpp"
#include <algorithm>
#include <cmath>

namespace GravyEngine
{
    float Mathf::Exp(float power)
    {
        return std::exp(power);
    }

    float Mathf::Min(float a, float b)
    {
        return std::min(a, b);
    }

    float Mathf::Max(float a, float b)
    {
        return std::max(a, b);
    }

    float Mathf::Abs(float value)
    {
        return std::abs(value);
    }

    float Mathf::Clamp(float value, float min, float max)
    {
        if(value < min)
            return min;
        else if(value > max)
            return max;
        return value;
    }

    float Mathf::Clamp01(float value)
    {
        return std::max(0.0f, std::min(value, 1.0f));
    }

    int32_t Mathf::FloorToInt(float value)
    {
        return static_cast<int32_t>(std::floor(value));
    }

    float Mathf::Floor(float value)
    {
        return std::floor(value);
    }

    float Mathf::InverseLerp(float start, float end, float value)
    {
        return (value - start) / (end - start);
    }

    float Mathf::InverseLerpClamped(float start, float end, float value)
    {
        return Mathf::Clamp((value - start) / (end - start), 0.0f, 1.0f);
    }

    float Mathf::Lerp(float start, float end, float percentage)
    {
        return start + ((end-start) * percentage);	
    }

    float Mathf::Sin(float x)
    {
        return std::sin(x);
    }

    float Mathf::Cos(float x)
    {
        return std::cos(x);
    }

    float Mathf::Sqrt(float x)
    {
        return std::sqrt(x);
    }

    float Mathf::Pow(float x, float y)
    {
        return std::pow(x, y);
    }

    float Mathf::Atan2(float x, float y)
    {
        return std::atan2(x, y);
    }

    float Mathf::Acos(float x)
    {
        return std::acos(x);
    }

    float Mathf::Sign(float value)
    {
        return value >= 0.0 ? 1.0f : -1.0f;
    }

    float Mathf::Tan(float value)
    {
        return std::tan(value);
    }

    float Mathf::DegreesToRadians(float degrees)
    {
        return Mathf::Deg2Rad * degrees;
    }

    float Mathf::RadiansToDegrees(float radians)
    {
        return Mathf::Rad2Deg * radians;
    }

    float Mathf::ClampPeriodicRange(float a, float min, float max)
    {
        float result = a;
        float range = (max - min);
        while(result >= max)
            result -= range;
        while(result < min)
            result += range;
        return result;
    }

    bool Mathf::IsNan(float value)
    {
        return std::isnan(value);
    }

    float Mathf::PerlinNoise(float x, float y)
    {
        return Noise::GetSample(x, y);
    }
}
