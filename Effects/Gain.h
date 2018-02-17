/*
  ==============================================================================

    Gain.h
    Created: 15 Feb 2018 4:18:43pm
    Author:  Jan Börner

  ==============================================================================
*/

#include "Effect.h"

#pragma once


namespace tsl
{
    namespace audioeffects
    {
        class Gain : public Effect
        {
        public:
            Gain(double sampleRate, int samplesPerBlock);
            ~Gain();
            void process(float* left, float* right, int numSamples);
            void process(float* mono, int numSamples);
            void setVol(float vol);
            float getVol();
        private:
            float vol;
        };
    }
}
