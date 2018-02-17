/*
  ==============================================================================

    Gain.cpp
    Created: 15 Feb 2018 4:18:43pm
    Author:  Jan Börner

  ==============================================================================
*/

#include "Gain.h"

namespace tsl
{
    namespace audioeffects
    {
        Gain::Gain(double sampleRate, int samplesPerBlock) : Effect(sampleRate, samplesPerBlock)
        {
            vol = 1;
        }
        Gain::~Gain()
        {
        }
        void Gain::process(float* left, float* right, int numSamples)
        {
            for(int i=0;i<samplesPerBlock;i++)
            {
                left[i] = left[i] * vol;
                right[i] = right[i] * vol;
            }
        }
        void Gain::process(float* mono, int numSamples)
        {
        }
        void Gain::setVol(float vol)
        {
            this->vol = vol;
        }
        float Gain::getVol()
        {
            return vol;
        }
    }
}
