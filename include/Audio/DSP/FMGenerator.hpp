#ifndef MINIAUDIOEX_FMGENERATOR_HPP
#define MINIAUDIOEX_FMGENERATOR_HPP

#include "AudioGenerator.hpp"
#include "Oscillator.hpp"
#include "../ConcurrentList.hpp"
#include <cstdint>

namespace GravyEngine
{
    class FMGenerator : public AudioGenerator
    {
    private:
        Oscillator carrier;
        ConcurrentList<Oscillator> operators;
    public:
        FMGenerator();
        FMGenerator(WaveType type, float frequency, float amplitude);
        int GetCount() const;
        Oscillator *operator[](int index);
        Oscillator *GetCarrier();
        Oscillator *GetOperator(size_t index);
        void Reset();
        void AddOperator(WaveType type, float frequency, float depth);
        void RemoveOperator(int index);
        void OnGenerate(AudioBuffer<float> framesOut, uint64_t frameCount, uint32_t channels) override;
        float GetModulatedSample();
    };
};

#endif