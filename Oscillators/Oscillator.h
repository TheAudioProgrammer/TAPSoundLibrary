/*
==============================================================================
Oscillator.h
Created: 12 Feb 2018 12:32:27am
Author:  Claudio
==============================================================================
*/

#include "WaveTable.h"
#include "Utils.h"

#pragma once
class Oscillator
{
public:
	Oscillator();
	Oscillator(float frequencyValue);
	~Oscillator();

	void setup() {
		setVolume(1.0);
		setSelectedWavetable(0);
		this->detuneSpreadMaxST = 12.0;
		updateVoicesIncrement();
	}

	/*
		This function is used to set the volume to the oscillator
	*/
	void setVolume(float volumeValue) {
		this->volume = volumeValue;
	}

	/*
		This function is used to get the volume of the oscillator
	*/
	float getVolume() {
		return this->volume;
	}

	/*
		This method read the value stored at position 'index' in the selected wavetable
	*/
	float getWavetableOut(int index) const { return wavetables.at(selectedWavetable).readAt(index); }

	/*
		This method is used to retrieve the selected WaveTable
	*/
	WaveTable getSelectedWavetable() {
		return wavetables.at(selectedWavetable);
	}

	/*
		This method is used to set the selected WaveTable
	*/
	void setSelectedWavetable(int selected) {
		this->selectedWavetable = selected;
	}

	/*
		This method is used to set the selected WaveTable
	*/
	void addWavetable(WaveTable wavetable) {
		this->wavetables.push_back(wavetable);
	}

	/*
		This function is used to set the frequency to the oscillator
	*/
	void setFrequency(float frequencyValue) {
		this->frequency = frequencyValue;
		updateVoicesIncrement();
	}

	/*
	This function is used to get the frequency of the oscillator
	*/
	float getFrequency() {
		return this->frequency;
	}

	/*
	This function is used to set the phase to this oscillator
	We will also set an index corresponding to which sample we have to read
	*/
	void setPhase(float phaseValue) {
		if (phaseValue >= 0.0 && phaseValue <= 360.0) {
			this->phase = phaseValue;
			phaseIndex = (int) Utils::scaleValue(phaseValue, 0.0, 360.0, 0.0, (float) getSelectedWavetable().getLength());
		}
	}

	void setMaxVoices(int maxVoices) {
		this->maxVoices = maxVoices;
		for (int i = 0; i < this->maxVoices; i++) {
			voicesOut.push_back(0.0);
			voicesPhase.push_back(0.0);
			voicesPhaseIncrement.push_back(0.0);
		}
	}

	/*
		This function is used to set the amount of voices of the oscillator
		This function will also replace the increment (to read the wavetable) each voice has to do if the detune is active
	*/
	void setVoices(int voices) {
		this->voices = voices;
	}

	/*
		This function is used to read the current voices value
	*/
	int getVoicesCount() {
		return this->voices;
	}

	/*
	This function is used to set the detune to this oscillator
	*/
	void setDetune(float detuneValue) {
		this->detune = detuneValue;
		updateVoicesIncrement();
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
			this->detuneSpread = detuneSpreadValue;
			updateVoicesIncrement();
		}
	}

	/*
	This function will be called in some cases
	Since detune is tuning near the current frequency we play, when we will change the frequency, we will also reset detune increments
	When we change detune spread, we have to update the increment
	When we change the detune value, we have to update the increment
	*/
	void updateVoicesIncrement() {
		if (this->voices > 0) {
			for (int i = 0; i < this->voices; i++) {
				int detuneIndex = (i + 1) - ((int)(this->voices + 0.5) / 2);
				float voiceFrequency = frequency + pow((detuneIndex * ((detune * detuneSpread * 10) / 1200)), 2.0);
				float voiceIncrement = voiceFrequency * getSelectedWavetable().getLength() / currentSampleRate;
				voicesPhaseIncrement.at(i) = voiceIncrement;
			}
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

	This method will use the passed frequency
	*/
	float getOutput(float frequency);
	/*
		Will use the current frequency of the oscillator if setted
	*/
	float getOutput();

private:
	float currentSampleRate = 44100;
	float frequency;

	std::vector<WaveTable> wavetables;
	int selectedWavetable;

	float volume;
	float phase;
	int phaseIndex;

	int maxVoices;
	int voices;
	std::vector<float> voicesOut;
	std::vector<float> voicesPhaseIncrement;
	std::vector<float> voicesPhase;

	float detune;
	float detuneSpread;
	float detunePosition;
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