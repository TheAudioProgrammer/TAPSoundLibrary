/*
  ==============================================================================

    Oscillator.h
    Created: 12 Feb 2018 12:32:27am
    Author:  Claudio

  ==============================================================================
*/

#include "WaveTable.h"

#pragma once
class Oscillator
{
	public:
		Oscillator();
		Oscillator();

		/*
			This function is used to set the phase to this oscillator
			We will also read
		*/
		void setPhase(float phaseValue) {
			if (phaseValue >= 0.0 && phaseValue <= 360.0) {
				phase = phaseValue;
				phaseIndex = (int) wave.scaleValue(phaseValue, 0.0, 360.0, 0.0, (float) wave.getLength());
			}
		}

		void setDetune(float detuneValue) {
			detune = detuneValue;
		}

		/*
			This function is used to set the detune spread
			We can spread the detune from 0.0 to our desired value

			To have a good spread we can use semitones, so all integer of the float (1.0, 2.0, 3.0...) will be semitones
			If we call setDetuneSpread(12.0) we will spread the detune to fill one octave because 12st = 1octave
			If we call setDetuneSpread(7.0) we will spread the detune to fill seven semitones

		*/
		void setDetuneSpread(float detuneSpreadValue) {
			if (detuneSpreadValue >= 0.0 && detuneSpreadValue <= detuneSpreadMaxST) {
				detuneSpread = detuneSpreadValue;
			}
		}

		/*
		_______________________________________ TODO __________________________________________________________________________________________
			This function will be the heart of our output, we will need to check these things
			1: Do we have voices != 0? If yes we can continue, otherwise we can output always 0
			2: Do we have volume > 0.0? If yes we can continue, otherwise always 0
			3: Do we have voices == 1? If yes, loop simply throught the wave
			4: Do we have voices > 1 && detune > 0.0 && detuneSpread == 0.0? If yes, we can loop through the wave
			5: Do we have voices > 1 && detune > 0.0 && detuneSpread > 0.0? If yes, we have to loop through the voices and calculate detune, then save them in voicesOut
			then sum all values up (stored in voicesOut)
		*/
		float getOutput(float frequency);

	private:

		WaveTable wave;
		float volume;
		float phase;
		int phaseIndex;

		int voices;
		std::vector<float> voicesOut;

		float detune;
		float detuneSpread;
		/*
			This variable keeps in itself how many semitones we can spread our detune
			The spread means how much can we detune starting from our frequency of the note played

			This value can be for example 12.0 or 24.0 or 28.1234

			Be sure that if you pass something like 12.632 you will be able to spread 12 semitones plus 0.632 semitones
		*/
		int detuneSpreadMaxST;

		float stereo;
		float pan;
		int octaves;
		int notes;
		float finetune;
};