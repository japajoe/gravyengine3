#ifndef MINIAUDIOEX_WAVECALCULATOR_HPP
#define MINIAUDIOEX_WAVECALCULATOR_HPP

namespace GravyEngine
{
    class WaveCalculator
    {
    public:
        virtual float GetValue(float phase)
        {
            return 0.0;
        }
    };
};

#endif