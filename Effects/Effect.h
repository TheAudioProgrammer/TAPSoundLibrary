/*
  ==============================================================================

    Effect.h
    Created: 12 Feb 2018 5:32:58pm
    Author:  Jan Börner

  ==============================================================================
*/

#pragma once

namespace tsl {
    
    namespace audioeffects
    
    {
        class Effect
        {
        public:
            Effect(double sampleRate, int samplesPerBlock);
            virtual ~Effect() = 0;
            
            void setSampleRate(double sampleRate);
            double getSampleRate();
            
            void setSamplesPerBlock(int samplesPerBlock);
            int getSamplesPerBlock();
            
            virtual float process(float* left, float* right) = 0;
            virtual float process(float* mono) = 0;
        protected:
            double sampleRate;
            int samplesPerBlock;
        };
    }
}
