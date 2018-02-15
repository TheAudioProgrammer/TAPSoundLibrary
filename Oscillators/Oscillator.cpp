/*
==============================================================================
Oscillator.cpp
Created: 12 Feb 2018 12:32:27am
Author:  Claudio
==============================================================================
*/

#include "Oscillator.h"

Oscillator::Oscillator()
{
}

Oscillator::Oscillator(float frequencyValue)
{
	setFrequency(frequencyValue);
}

Oscillator::~Oscillator()
{
}

float Oscillator::getOutput(float frequency)
{
	if (this->frequency != frequency) {
		setFrequency(frequency);
	}
	float out = 0.0f;
	if (this->voices == 0 || this->volume == 0.0) {
		return out;
	}
	if (this->volume > 0.0 && this->voices > 1) {
		for (int i = 0; i < this->voices; i++) {
			out = out + this->getSelectedWavetable().readAt((int)this->voicesPhaseIncrement.at(i)) * this->volume;
			this->voicesPhase.at(i) = fmod((this->voicesPhase.at(i) + (int) this->voicesPhaseIncrement.at(i)), this->getSelectedWavetable().getLength());
		}
		out = out / this->voices;
	}
	return 0.0f;
}

float Oscillator::getOutput()
{
	float out = 0.0f;
	if (this->voices == 0 || this->volume == 0.0) {
		return out;
	}
	if (this->volume > 0.0 && this->voices > 0) {
		if (wavetables.size() > 0) {
			if (detune > 0.0) {
				for (int i = 0; i < this->voices; i++) {
					out = out + this->getSelectedWavetable().readAt((int)this->voicesPhase.at(i));
					this->voicesPhase.at(i) = fmod((this->voicesPhase.at(i) + (int) this->voicesPhaseIncrement.at(i)), this->getSelectedWavetable().getLength());
				}
				out = out / this->voices;
			}
			else {
				out = out + this->getSelectedWavetable().readAt((int)this->voicesPhase.at(0));
				this->voicesPhase.at(0) = fmod((this->voicesPhase.at(0) + (int) this->voicesPhaseIncrement.at(0)), this->getSelectedWavetable().getLength());
			}
			out = out * this->volume;
		}
	}
	return out;
}