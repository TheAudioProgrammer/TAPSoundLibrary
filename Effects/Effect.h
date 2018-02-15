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
        /**
         * An abstract class to define how to implement an audio effect.
         */
        class Effect
        {
        public:
            /**
             * Constructor which takes the sampleRate and
             * the samplesPerBlock. You need to call this
             * in you sub class.
             *
             * @param sampleRate
             * @param samplesPerBlock
             */
            Effect(double sampleRate, int samplesPerBlock);
            /**
             * Sets the samplerate.
             *
             * @param sampleRate
             */
            void setSampleRate(double sampleRate);
            /**
             * Returns the samplerate the plugin is using.
             *
             * @return
             */
            double getSampleRate();
            /**
             * Sets the samples per block rate.
             *
             * @param samplesPerBlock
             */
            void setSamplesPerBlock(int samplesPerBlock);
            /**
             * Returns the samples per block rate.
             *
             * @return
             */
            int getSamplesPerBlock();
            /**
             * This is where you process the audio if your effect
             * is stereo. You need to implement this method in your
             * supclass.
             *
             * @param left
             * @param right
             */
            virtual void process(float* left, float* right) = 0;
            /**
             * This is where you process the audio if your effect
             * is mono. You need to implement this method in your
             * supclass.
             *
             * @param mono
             */
            virtual void process(float* mono) = 0;
        protected:
            /**
             * The number of samples your system
             * process in one second.
             */
            double sampleRate;
            /**
             * How much samples are passed during any
             * call of the proccess method.
             */
            int samplesPerBlock;
        };
    }
}
