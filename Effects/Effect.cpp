/*
  ==============================================================================

    Effect.cpp
    Created: 12 Feb 2018 5:32:58pm
    Author:  Jan Börner

  ==============================================================================
*/

#include "Effect.h"

namespace tsl {
    
    namespace audioeffects {
    
        Effect::Effect(double sampleRate, int samplesPerBlock)
        {
            this->sampleRate = sampleRate;
            this->samplesPerBlock = samplesPerBlock;
        }
        double Effect::getSampleRate()
        {
            return this->sampleRate;
        }
        void Effect::setSampleRate(double sampleRate)
        {
            this->sampleRate = sampleRate;
        }
        void Effect::setSamplesPerBlock(int samplesPerBlock)
        {
            this->samplesPerBlock = samplesPerBlock;
        }
        int Effect::getSamplesPerBlock()
        {
            return this->samplesPerBlock;
        }
        
    }
}
