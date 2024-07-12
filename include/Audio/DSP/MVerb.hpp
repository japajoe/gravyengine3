//	Copyright (c) 2010 Martin Eastwood
//  This code is distributed under the terms of the GNU General Public License

//  MVerb is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  at your option) any later version.
//
//  MVerb is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this MVerb.  If not, see <http://www.gnu.org/licenses/>.

#ifndef GRAVYENGINE_MVERB_HPP
#define GRAVYENGINE_MVERB_HPP

#include <cmath>
#include <cstring>

namespace GravyEngine
{
    //forward declaration
    template<typename T, int maxLength> class Allpass;
    template<typename T, int maxLength> class StaticAllpassFourTap;
    template<typename T, int maxLength> class StaticDelayLine;
    template<typename T, int maxLength> class StaticDelayLineFourTap;
    template<typename T, int maxLength> class StaticDelayLineEightTap;
    template<typename T, int OverSampleCount> class StateVariable;

    template<typename T>
    class MVerb
    {
    private:
        Allpass<T, 44100> allpass[4] = {};
        StaticAllpassFourTap<T, 44100> allpassFourTap[4] = {};
        StateVariable<T,4> bandwidthFilter[2] = {};
        StateVariable<T,4> damping[2] = {};
        StaticDelayLine<T, 44100> predelay = {};
        StaticDelayLineFourTap<T, 44100> staticDelayLine[4] = {};
        StaticDelayLineEightTap<T, 44100> earlyReflectionsDelayLine[2] = {};
        T SampleRate = {};
        T DampingFreq = {};
        T Density1 = {};
        T Density2 = {};
        T BandwidthFreq = {};
        T PreDelayTime = {};
        T Decay = {};
        T Gain = {};
        T Mix = {};
        T EarlyMix = {};
        T Size = {};

        T MixSmooth = {};
        T EarlyLateSmooth = {};
        T BandwidthSmooth = {};
        T DampingSmooth = {};
        T PredelaySmooth = {};
        T SizeSmooth = {};
        T DensitySmooth = {};
        T DecaySmooth = {};

        T PreviousLeftTank = {};
        T PreviousRightTank = {};

        int ControlRate = 0;
        int ControlRateCounter = 0;

    public:
        enum
            {
                DAMPINGFREQ=0,
                DENSITY,
                BANDWIDTHFREQ,
                DECAY,
                PREDELAY,
                SIZE,
                GAIN,
                MIX,
                EARLYMIX,
                NUM_PARAMS
            };

        MVerb(){
            DampingFreq = 0.9;
            BandwidthFreq = 0.9;
            SampleRate = 44100.;
            Decay = 0.5;
            Gain = 1.;
            Mix = 1.;
            Size = 1.;
            EarlyMix = 1.;
            PreviousLeftTank = 0.;
            PreviousRightTank = 0.;
            PreDelayTime = 100 * (SampleRate / 1000);
            MixSmooth = EarlyLateSmooth = BandwidthSmooth = DampingSmooth = PredelaySmooth = SizeSmooth = DecaySmooth = DensitySmooth = 0.;
            ControlRate = static_cast<int>(SampleRate / 1000);
            ControlRateCounter = 0;
            reset();
        }

        ~MVerb(){
            //nowt to do here
        }

        void process(T *inputs, T *outputs, int sampleFrames, int channels){
            T OneOverSampleFrames = static_cast<T>(1. / sampleFrames);
            T MixDelta	= (Mix - MixSmooth) * OneOverSampleFrames;
            T EarlyLateDelta = (EarlyMix - EarlyLateSmooth) * OneOverSampleFrames;
            T BandwidthDelta = static_cast<T>((((BandwidthFreq * 18400.) + 100.) - BandwidthSmooth) * OneOverSampleFrames);
            T DampingDelta = static_cast<T>((((DampingFreq * 18400.) + 100.) - DampingSmooth) * OneOverSampleFrames);
            T PredelayDelta = static_cast<T>(((PreDelayTime * 200 * (SampleRate / 1000)) - PredelaySmooth) * OneOverSampleFrames);
            T SizeDelta	= static_cast<T>((Size - SizeSmooth) * OneOverSampleFrames);
            T DecayDelta = static_cast<T>((((0.7995f * Decay) + 0.005) - DecaySmooth) * OneOverSampleFrames);
            T DensityDelta = static_cast<T>((((0.7995f * Density1) + 0.005) - DensitySmooth) * OneOverSampleFrames);
            int index = 0;
            for(int i=0;i<sampleFrames;++i){
                T left = inputs[index];
                T right = inputs[index+1];
                MixSmooth += MixDelta;
                EarlyLateSmooth += EarlyLateDelta;
                BandwidthSmooth += BandwidthDelta;
                DampingSmooth += DampingDelta;
                PredelaySmooth += PredelayDelta;
                SizeSmooth += SizeDelta;
                DecaySmooth += DecayDelta;
                DensitySmooth += DensityDelta;
                if (ControlRateCounter >= ControlRate){
                    ControlRateCounter = 0;
                    bandwidthFilter[0].Frequency(BandwidthSmooth);
                    bandwidthFilter[1].Frequency(BandwidthSmooth);
                    damping[0].Frequency(DampingSmooth);
                    damping[1].Frequency(DampingSmooth);
                }
                ++ControlRateCounter;
                predelay.SetLength(static_cast<int>(PredelaySmooth));
                Density2 = static_cast<T>(DecaySmooth + 0.15);
                if (Density2 > 0.5)
                    Density2 = 0.5;
                if (Density2 < 0.25)
                    Density2 = 0.25;
                allpassFourTap[1].SetFeedback(Density2);
                allpassFourTap[3].SetFeedback(Density2);
                allpassFourTap[0].SetFeedback(Density1);
                allpassFourTap[2].SetFeedback(Density1);
                T bandwidthLeft = bandwidthFilter[0](left) ;
                T bandwidthRight = bandwidthFilter[1](right) ;
                T earlyReflectionsL = static_cast<T>(earlyReflectionsDelayLine[0] ( bandwidthLeft * 0.5 + bandwidthRight * 0.3 )
                                    + earlyReflectionsDelayLine[0].GetIndex(2) * 0.6
                                    + earlyReflectionsDelayLine[0].GetIndex(3) * 0.4
                                    + earlyReflectionsDelayLine[0].GetIndex(4) * 0.3
                                    + earlyReflectionsDelayLine[0].GetIndex(5) * 0.3
                                    + earlyReflectionsDelayLine[0].GetIndex(6) * 0.1
                                    + earlyReflectionsDelayLine[0].GetIndex(7) * 0.1
                                    + ( bandwidthLeft * 0.4 + bandwidthRight * 0.2 ) * 0.5);
                T earlyReflectionsR = static_cast<T>(earlyReflectionsDelayLine[1] ( bandwidthLeft * 0.3 + bandwidthRight * 0.5 )
                                    + earlyReflectionsDelayLine[1].GetIndex(2) * 0.6
                                    + earlyReflectionsDelayLine[1].GetIndex(3) * 0.4
                                    + earlyReflectionsDelayLine[1].GetIndex(4) * 0.3
                                    + earlyReflectionsDelayLine[1].GetIndex(5) * 0.3
                                    + earlyReflectionsDelayLine[1].GetIndex(6) * 0.1
                                    + earlyReflectionsDelayLine[1].GetIndex(7) * 0.1
                                    + ( bandwidthLeft * 0.2 + bandwidthRight * 0.4 ) * 0.5);
                T predelayMonoInput = predelay(( bandwidthRight + bandwidthLeft ) * 0.5f);
                T smearedInput = predelayMonoInput;
                for(int j=0;j<4;j++)
                    smearedInput = allpass[j] ( smearedInput );
                T leftTank = allpassFourTap[0] ( smearedInput + PreviousRightTank ) ;
                leftTank = staticDelayLine[0] (leftTank);
                leftTank = damping[0](leftTank);
                leftTank = allpassFourTap[1](leftTank);
                leftTank = staticDelayLine[1](leftTank);
                T rightTank = allpassFourTap[2] (smearedInput + PreviousLeftTank) ;
                rightTank = staticDelayLine[2](rightTank);
                rightTank = damping[1] (rightTank);
                rightTank = allpassFourTap[3](rightTank);
                rightTank = staticDelayLine[3](rightTank);
                PreviousLeftTank = leftTank * DecaySmooth;
                PreviousRightTank = rightTank * DecaySmooth;
                T accumulatorL = static_cast<T>((0.6*staticDelayLine[2].GetIndex(1))
                                +(0.6*staticDelayLine[2].GetIndex(2))
                                -(0.6*allpassFourTap[3].GetIndex(1))
                                +(0.6*staticDelayLine[3].GetIndex(1))
                                -(0.6*staticDelayLine[0].GetIndex(1))
                                -(0.6*allpassFourTap[1].GetIndex(1))
                                -(0.6*staticDelayLine[1].GetIndex(1)));
                T accumulatorR = static_cast<T>((0.6*staticDelayLine[0].GetIndex(2))
                                +(0.6*staticDelayLine[0].GetIndex(3))
                                -(0.6*allpassFourTap[1].GetIndex(2))
                                +(0.6*staticDelayLine[1].GetIndex(2))
                                -(0.6*staticDelayLine[2].GetIndex(3))
                                -(0.6*allpassFourTap[3].GetIndex(2))
                                -(0.6*staticDelayLine[3].GetIndex(2)));
                accumulatorL = ((accumulatorL * EarlyMix) + ((1 - EarlyMix) * earlyReflectionsL));
                accumulatorR = ((accumulatorR * EarlyMix) + ((1 - EarlyMix) * earlyReflectionsR));
                left = ( left + MixSmooth * ( accumulatorL - left ) ) * Gain;
                right = ( right + MixSmooth * ( accumulatorR - right ) ) * Gain;
                
                outputs[index] = left;
                if(channels == 2)
                    outputs[index+1] = right;

                index += channels;
            }
        }

        void reset(){
            ControlRateCounter = 0;
            bandwidthFilter[0].SetSampleRate(SampleRate);
            bandwidthFilter[1].SetSampleRate(SampleRate);
            bandwidthFilter[0].Reset();
            bandwidthFilter[1].Reset();
            damping[0].SetSampleRate(SampleRate);
            damping[1].SetSampleRate(SampleRate);
            damping[0].Reset();
            damping[1].Reset();
            predelay.Clear();
            predelay.SetLength(static_cast<int>(PreDelayTime));
            allpass[0].Clear();
            allpass[1].Clear();
            allpass[2].Clear();
            allpass[3].Clear();
            allpass[0].SetLength(static_cast<int>(0.0048 * SampleRate));
            allpass[1].SetLength(static_cast<int>(0.0036 * SampleRate));
            allpass[2].SetLength(static_cast<int>(0.0127 * SampleRate));
            allpass[3].SetLength(static_cast<int>(0.0093 * SampleRate));
            allpass[0].SetFeedback(0.75);
            allpass[1].SetFeedback(0.75);
            allpass[2].SetFeedback(0.625);
            allpass[3].SetFeedback(0.625);
            allpassFourTap[0].Clear();
            allpassFourTap[1].Clear();
            allpassFourTap[2].Clear();
            allpassFourTap[3].Clear();
            allpassFourTap[0].SetLength(static_cast<int>(0.020 * SampleRate * Size));
            allpassFourTap[1].SetLength(static_cast<int>(0.060 * SampleRate * Size));
            allpassFourTap[2].SetLength(static_cast<int>(0.030 * SampleRate * Size));
            allpassFourTap[3].SetLength(static_cast<int>(0.089 * SampleRate * Size));
            allpassFourTap[0].SetFeedback(Density1);
            allpassFourTap[1].SetFeedback(Density2);
            allpassFourTap[2].SetFeedback(Density1);
            allpassFourTap[3].SetFeedback(Density2);
            allpassFourTap[0].SetIndex(0, 0, 0, 0);
            allpassFourTap[1].SetIndex(0, static_cast<int>(0.006 * SampleRate * Size), static_cast<int>(0.041 * SampleRate * Size), 0);
            allpassFourTap[2].SetIndex(0, 0, 0, 0);
            allpassFourTap[3].SetIndex(0, static_cast<int>(0.031 * SampleRate * Size), static_cast<int>(0.011 * SampleRate * Size), 0);
            staticDelayLine[0].Clear();
            staticDelayLine[1].Clear();
            staticDelayLine[2].Clear();
            staticDelayLine[3].Clear();
            staticDelayLine[0].SetLength(static_cast<int>(0.15 * SampleRate * Size));
            staticDelayLine[1].SetLength(static_cast<int>(0.12 * SampleRate * Size));
            staticDelayLine[2].SetLength(static_cast<int>(0.14 * SampleRate * Size));
            staticDelayLine[3].SetLength(static_cast<int>(0.11 * SampleRate * Size));
            staticDelayLine[0].SetIndex(0, static_cast<int>(0.067 * SampleRate * Size), static_cast<int>(0.011 * SampleRate * Size), static_cast<int>(0.121 * SampleRate * Size));
            staticDelayLine[1].SetIndex(0, static_cast<int>(0.036 * SampleRate * Size), static_cast<int>(0.089 * SampleRate * Size), 0);
            staticDelayLine[2].SetIndex(0, static_cast<int>(0.0089 * SampleRate * Size), static_cast<int>(0.099 * SampleRate * Size), 0);
            staticDelayLine[3].SetIndex(0, static_cast<int>(0.067 * SampleRate * Size), static_cast<int>(0.0041 * SampleRate * Size), 0);
            earlyReflectionsDelayLine[0].Clear();
            earlyReflectionsDelayLine[1].Clear();
            earlyReflectionsDelayLine[0].SetLength(static_cast<int>(0.089 * SampleRate));
            earlyReflectionsDelayLine[0].SetIndex(0, static_cast<int>(0.0199 * SampleRate), static_cast<int>(0.0219 * SampleRate), static_cast<int>(0.0354 * SampleRate), static_cast<int>(0.0389 * SampleRate), static_cast<int>(0.0414 * SampleRate), static_cast<int>(0.0692 * SampleRate), 0);
            earlyReflectionsDelayLine[1].SetLength(static_cast<int>(0.069 * SampleRate));
            earlyReflectionsDelayLine[1].SetIndex(0, static_cast<int>(0.0099 * SampleRate), static_cast<int>(0.011 * SampleRate), static_cast<int>(0.0182 * SampleRate), static_cast<int>(0.0189 * SampleRate), static_cast<int>(0.0213 * SampleRate), static_cast<int>(0.0431 * SampleRate), 0);
        }

        void setParameter(int index, T value){
            switch(index){
                case DAMPINGFREQ:
                        DampingFreq = static_cast<T>(1. - value);
                        break;
                case DENSITY:
                        Density1 = value;
                        break;
                case BANDWIDTHFREQ:
                        BandwidthFreq = value;
                        break;
                case PREDELAY:
                        PreDelayTime = value;
                        break;
                case SIZE:
                        Size = static_cast<T>((0.95 * value) + 0.05);
                        allpassFourTap[0].Clear();
                        allpassFourTap[1].Clear();
                        allpassFourTap[2].Clear();
                        allpassFourTap[3].Clear();
                        allpassFourTap[0].SetLength(static_cast<int>(0.020 * SampleRate * Size));
                        allpassFourTap[1].SetLength(static_cast<int>(0.060 * SampleRate * Size));
                        allpassFourTap[2].SetLength(static_cast<int>(0.030 * SampleRate * Size));
                        allpassFourTap[3].SetLength(static_cast<int>(0.089 * SampleRate * Size));
                        allpassFourTap[1].SetIndex(0, static_cast<int>(0.006 * SampleRate * Size), static_cast<int>(0.041 * SampleRate * Size), 0);
                        allpassFourTap[3].SetIndex(0, static_cast<int>(0.031 * SampleRate * Size), static_cast<int>(0.011 * SampleRate * Size), 0);
                        staticDelayLine[0].Clear();
                        staticDelayLine[1].Clear();
                        staticDelayLine[2].Clear();
                        staticDelayLine[3].Clear();
                        staticDelayLine[0].SetLength(static_cast<int>(0.15 * SampleRate * Size));
                        staticDelayLine[1].SetLength(static_cast<int>(0.12 * SampleRate * Size));
                        staticDelayLine[2].SetLength(static_cast<int>(0.14 * SampleRate * Size));
                        staticDelayLine[3].SetLength(static_cast<int>(0.11 * SampleRate * Size));
                        staticDelayLine[0].SetIndex(0, static_cast<int>(0.067 * SampleRate * Size), static_cast<int>(0.011 * SampleRate * Size), static_cast<int>(0.121 * SampleRate * Size));
                        staticDelayLine[1].SetIndex(0, static_cast<int>(0.036 * SampleRate * Size), static_cast<int>(0.089 * SampleRate * Size), 0);
                        staticDelayLine[2].SetIndex(0, static_cast<int>(0.0089 * SampleRate * Size), static_cast<int>(0.099 * SampleRate * Size), 0);
                        staticDelayLine[3].SetIndex(0, static_cast<int>(0.067 * SampleRate * Size), static_cast<int>(0.0041 * SampleRate * Size), 0);
                        break;
                case DECAY:
                        Decay = value;
                        break;
                case GAIN:
                        Gain = value;
                        break;
                case MIX:
                        Mix = value;
                        break;
                case EARLYMIX:
                        EarlyMix = value;
                        break;
            }
        }

        float getParameter(int index) const{
            switch(index){
                case DAMPINGFREQ:
                        return DampingFreq * 100.;
                        break;
                case DENSITY:
                        return Density1 * 100.f;
                        break;
                case BANDWIDTHFREQ:
                        return BandwidthFreq * 100.;
                        break;
                case PREDELAY:
                        return PreDelayTime * 100.;
                        break;
                case SIZE:
                        return (((0.95 * Size) + 0.05)*100.);
                        break;
                case DECAY:
                        return Decay * 100.f;
                        break;
                case GAIN:
                        return Gain * 100.f;
                        break;
                case MIX:
                        return Mix * 100.f;
                        break;
                case EARLYMIX:
                        return EarlyMix * 100.f;
                        break;
                default: return 0.f;
                    break;

            }
        }

        void setSampleRate(T sr){
            SampleRate = sr;
            ControlRate = static_cast<int>(SampleRate / 1000);
            reset();
        }

        void setDampingFrequency(float value)
        {
            setParameter(DAMPINGFREQ, value);
        }

        float getDampingFrequency() const
        {
            return getParameter(DAMPINGFREQ);
        }

        void setDensity(float value)
        {
            setParameter(DENSITY, value);
        }

        float getDensity() const
        {
            return getParameter(DENSITY);
        }

        void setBandwidthFrequency(float value)
        {
            setParameter(BANDWIDTHFREQ, value);
        }

        float getBandwidthFrequency() const
        {
            return getParameter(BANDWIDTHFREQ);
        }

        void setPreDelayTime(float value)
        {
            setParameter(PREDELAY, value);
        }

        float getPreDelayTime() const
        {
            return getParameter(PREDELAY);
        }

        void setSize(float value)
        {
            setParameter(SIZE, value);
        }

        float getSize() const
        {
            return getParameter(SIZE);
        }

        void setDecay(float value)
        {
            setParameter(DECAY, value);
        }

        float getDecay() const
        {
            return getParameter(DECAY);
        }

        void setGain(float value)
        {
            setParameter(GAIN, value);
        }

        float getGain() const
        {
            return getParameter(GAIN);
        }

        void setMix(float value)
        {
            setParameter(MIX, value);
        }

        float getMix() const
        {
            return getParameter(MIX);
        }

        void setEarlyMix(float value)
        {
            setParameter(EARLYMIX, value);
        }

        float getEarlyMix() const
        {
            return getParameter(EARLYMIX);
        }

        // void loadPreset(const MVerbPreset &preset)
        // {
        //     setDampingFrequency(preset.dampingFrequency);
        //     setDensity(preset.density);
        //     setBandwidthFrequency(preset.bandwidthFrequency);
        //     setPreDelayTime(preset.preDelayTime);
        //     setSize(preset.size);
        //     setDecay(preset.decay);
        //     setGain(preset.gain);
        //     setMix(preset.mix);
        //     setEarlyMix(preset.earlyMix);
        // }
    };



    template<typename T, int maxLength>
    class Allpass
    {
    private:
        T buffer[maxLength] = {};
        int index = 0;
        int Length = 0;
        T Feedback = {};

    public:
        Allpass()
        {
            SetLength ( maxLength - 1 );
            Clear();
            Feedback = 0.5;
        }

        T operator()(T input)
        {
            T output;
            T bufout;
            bufout = buffer[index];
            T temp = input * -Feedback;
            output = bufout + temp;
            buffer[index] = input + ((bufout+temp)*Feedback);
            if(++index>=Length) index = 0;
            return output;

        }

        void SetLength (int Length)
        {
        if( Length >= maxLength )
                Length = maxLength;
        if( Length < 0 )
                Length = 0;

            this->Length = Length;
        }

        void SetFeedback(T feedback)
        {
            Feedback = feedback;
        }

        void Clear()
        {
            memset(buffer, 0, sizeof(buffer));
            index = 0;
        }

        int GetLength() const
        {
            return Length;
        }
    };

    template<typename T, int maxLength>
    class StaticAllpassFourTap
    {
    private:
        T buffer[maxLength] = {};
        int index1 = 0;
        int index2 = 0;
        int index3 = 0;
        int index4 = 0;
        int Length = 0;
        T Feedback = {};

    public:
        StaticAllpassFourTap()
        {
            SetLength ( maxLength - 1 );
            Clear();
            Feedback = 0.5;
        }

        T operator()(T input)
        {
            T output;
            T bufout;

            bufout = buffer[index1];
            T temp = input * -Feedback;
            output = bufout + temp;
            buffer[index1] = input + ((bufout+temp)*Feedback);

            if(++index1>=Length)
                index1 = 0;
            if(++index2 >= Length)
                index2 = 0;
            if(++index3 >= Length)
                index3 = 0;
            if(++index4 >= Length)
                index4 = 0;

            return output;

        }

        void SetIndex (int Index1, int Index2, int Index3, int Index4)
        {
            index1 = Index1;
            index2 = Index2;
            index3 = Index3;
            index4 = Index4;
        }

        T GetIndex (int Index)
        {
            switch (Index)
            {
                case 0:
                    return buffer[index1];
                    break;
                case 1:
                    return buffer[index2];
                    break;
                case 2:
                    return buffer[index3];
                    break;
                case 3:
                    return buffer[index4];
                    break;
                default:
                    return buffer[index1];
                    break;
            }
        }

        void SetLength (int Length)
        {
        if( Length >= maxLength )
                Length = maxLength;
        if( Length < 0 )
                Length = 0;

            this->Length = Length;
        }


        void Clear()
        {
            memset(buffer, 0, sizeof(buffer));
            index1 = index2  = index3 = index4 = 0;
        }

        void SetFeedback(T feedback)
        {
            Feedback = feedback;
        }


        int GetLength() const
        {
            return Length;
        }
    };

    template<typename T, int maxLength>
    class StaticDelayLine
    {
    private:
        T buffer[maxLength] = {};
        int index = 0;
        int Length = 0;
        T Feedback = {};

    public:
        StaticDelayLine()
        {
            SetLength ( maxLength - 1 );
            Clear();
        }

        T operator()(T input)
        {
            T output = buffer[index];
            buffer[index++] = input;
            if(index >= Length)
                index = 0;
            return output;

        }

        void SetLength (int Length)
        {
        if( Length >= maxLength )
                Length = maxLength;
        if( Length < 0 )
                Length = 0;

            this->Length = Length;
        }

        void Clear()
        {
            memset(buffer, 0, sizeof(buffer));
            index = 0;
        }

        int GetLength() const
        {
            return Length;
        }
    };

    template<typename T, int maxLength>
    class StaticDelayLineFourTap
    {
    private:
        T buffer[maxLength] = {};
        int index1 = 0;
        int index2 = 0;
        int index3 = 0;
        int index4 = 0;
        int Length = 0;
        T Feedback = {};

    public:
        StaticDelayLineFourTap()
        {
            SetLength ( maxLength - 1 );
            Clear();
        }

        //get ouput and iterate
        T operator()(T input)
        {
            T output = buffer[index1];
            buffer[index1++] = input;
            if(index1 >= Length)
                index1 = 0;
            if(++index2 >= Length)
                index2 = 0;
            if(++index3 >= Length)
                index3 = 0;
            if(++index4 >= Length)
                index4 = 0;
            return output;

        }

        void SetIndex (int Index1, int Index2, int Index3, int Index4)
        {
            index1 = Index1;
            index2 = Index2;
            index3 = Index3;
            index4 = Index4;
        }


        T GetIndex (int Index)
        {
            switch (Index)
            {
                case 0:
                    return buffer[index1];
                    break;
                case 1:
                    return buffer[index2];
                    break;
                case 2:
                    return buffer[index3];
                    break;
                case 3:
                    return buffer[index4];
                    break;
                default:
                    return buffer[index1];
                    break;
            }
        }


        void SetLength (int Length)
        {
        if( Length >= maxLength )
                Length = maxLength;
        if( Length < 0 )
                Length = 0;

            this->Length = Length;
        }


        void Clear()
        {
            memset(buffer, 0, sizeof(buffer));
            index1 = index2  = index3 = index4 = 0;
        }


        int GetLength() const
        {
            return Length;
        }
    };

    template<typename T, int maxLength>
    class StaticDelayLineEightTap
    {
    private:
        T buffer[maxLength] = {};
        int index1 = 0;
        int index2 = 0;
        int index3 = 0;
        int index4 = 0;
        int index5 = 0;
        int index6 = 0;
        int index7 = 0;
        int index8 = 0;
        int Length = 0;
        T Feedback = {};

    public:
        StaticDelayLineEightTap()
        {
            SetLength ( maxLength - 1 );
            Clear();
        }

        //get ouput and iterate
        T operator()(T input)
        {
            T output = buffer[index1];
            buffer[index1++] = input;
            if(index1 >= Length)
                index1 = 0;
            if(++index2 >= Length)
                index2 = 0;
            if(++index3 >= Length)
                index3 = 0;
            if(++index4 >= Length)
                index4 = 0;
            if(++index5 >= Length)
                index5 = 0;
            if(++index6 >= Length)
                index6 = 0;
            if(++index7 >= Length)
                index7 = 0;
            if(++index8 >= Length)
                index8 = 0;
            return output;

        }

        void SetIndex (int Index1, int Index2, int Index3, int Index4, int Index5, int Index6, int Index7, int Index8)
        {
            index1 = Index1;
            index2 = Index2;
            index3 = Index3;
            index4 = Index4;
            index5 = Index5;
            index6 = Index6;
            index7 = Index7;
            index8 = Index8;
        }


        T GetIndex (int Index)
        {
            switch (Index)
            {
                case 0:
                    return buffer[index1];
                    break;
                case 1:
                    return buffer[index2];
                    break;
                case 2:
                    return buffer[index3];
                    break;
                case 3:
                    return buffer[index4];
                    break;
                case 4:
                    return buffer[index5];
                    break;
                case 5:
                    return buffer[index6];
                    break;
                case 6:
                    return buffer[index7];
                    break;
                case 7:
                    return buffer[index8];
                    break;
                default:
                    return buffer[index1];
                    break;
            }
        }


        void SetLength (int Length)
        {
        if( Length >= maxLength )
                Length = maxLength;
        if( Length < 0 )
                Length = 0;

            this->Length = Length;
        }


        void Clear()
        {
            memset(buffer, 0, sizeof(buffer));
            index1 = index2  = index3 = index4 = index5 = index6 = index7 = index8 = 0;
        }


        int GetLength() const
        {
            return Length;
        }
    };

    template<typename T, int OverSampleCount>
    class StateVariable
    {
    public:

        enum FilterType
        {
            LOWPASS,
            HIGHPASS,
            BANDPASS,
            NOTCH,
            FilterTypeCount
        };

    private:

        T sampleRate = {};
        T frequency = {};
        T q = {};
        T f = {};

        T low = {};
        T high = {};
        T band = {};
        T notch = {};

        T *out = nullptr;

    public:
        StateVariable()
        {
            SetSampleRate(44100.);
            Frequency(1000.);
            Resonance(0);
            Type(LOWPASS);
            Reset();
        }

        T operator()(T input)
        {
            for(unsigned int i = 0; i < OverSampleCount; i++)
            {
                low += static_cast<T>(f * band + 1e-25);
                high = input - low - q * band;
                band += f * high;
                notch = low + high;
            }
			return *out;
        }

        void Reset()
        {
            low = high = band = notch = 0;
        }

        void SetSampleRate(T sampleRate)
        {
            this->sampleRate = sampleRate * OverSampleCount;
            UpdateCoefficient();
        }

        void Frequency(T frequency)
        {
            this->frequency = frequency;
            UpdateCoefficient();
        }

        void Resonance(T resonance)
        {
            this->q = 2 - 2 * resonance;
        }

        void Type(int type)
        {
            switch(type)
            {
            case LOWPASS:
                out = &low;
                break;

            case HIGHPASS:
                out = &high;
                break;

            case BANDPASS:
                out = &band;
                break;

            case NOTCH:
                out = &notch;
                break;

            default:
                out = &low;
                break;
            }
        }

    private:
        void UpdateCoefficient()
        {
            f = static_cast<T>(2. * sinf(3.141592654 * frequency / sampleRate));
        }
	};
};

#endif