/*
  ==============================================================================

    WaveTable.cpp
    Created: 11 Feb 2018 11:19:46pm
    Author:  Claudio

  ==============================================================================
*/

#include "WaveTable.h"
#include <algorithm>


WaveTable::WaveTable()
{
}


WaveTable::WaveTable(int length)
{
	content = std::vector<float>(length);
}

WaveTable::~WaveTable()
{
}

void WaveTable::setLength(int length)
{
	//We create a vector of the current length;
	std::vector<float> newContent(length);
	int currentLength = getLength();
	//If our current content at least has 1 value or more
	if (currentLength > 0) {

	}
}

void WaveTable::cutAboveRange(float range)
{
	if (range >= 0.0) {
		//We loop through the wavetable
		for (int i = 0; i < getLength(); i++) {
			//We get the current value of the wavetable with content.at[i] where "i" is an integer
			//If the current value exceeds the range we can set the value to take tha range value
			if (content.at(i) > range) {
				content.at(i) = range;
			}
			if (content.at(i) < (range * -1)) {
				content.at(i) = range * -1;
			}
		}
	}
}

std::vector<float> WaveTable::readCutAboveRange(float range)
{
	std::vector<float> readWave = content;
	if (range >= 0.0) {
		for (int i = 0; i < readWave.size(); i++) {
			//We get the current value of the wavetable with content.at[i] where "i" is an integer
			//If the current value exceeds the range we can set the value to take tha range value
			if (readWave.at(i) > range) {
				readWave.at(i) = range;
			}
			if (readWave.at(i) < (range * -1)) {
				readWave.at(i) = range * -1;
			}
		}
	}
	return readWave;
}

void WaveTable::invertWavetable(float invert)
{
	//If our invert has a value between -1.0 and 1.0
	if (invert >= -1.0 && invert <= 1.0) {
		//If it equals 1.0 we will not have to affect the wavetable
		if (invert != 1.0) {
			//We loop through the wavetable
			for (int i = 0; i < getLength(); i++) {
				content.at(i) = content.at(i) * invert;
			}
		}
	}
}

std::vector<float> WaveTable::readInvertWavetable(float invert)
{
	std::vector<float> readWave = content;
	//If our invert has a value between -1.0 and 1.0
	if (invert >= -1.0 && invert <= 1.0) {
		//If it equals 1.0 we will not have to affect the wavetable
		if (invert != 1.0) {
			//We loop through the wavetable content and we read it
			for (int i = 0; i < readWave.size(); i++) {
				readWave.at(i) = readWave.at(i) * invert;
			}
		}
	}
	return readWave;
}

void WaveTable::scaleWavetable(float newMin, float newMax)
{
	//We get the current min and max values of our wavetable (not yet scaled)
	float currentMin = *std::min_element(content.begin(), content.end());
	float currentMax = *std::max_element(content.begin(), content.end());
	//We loop through the wavetable
	for (int i = 0; i < getLength(); i++) {
		content.at(i) = scaleValue(content.at(i), currentMin, currentMax, newMin, newMax);
	}
}

std::vector<float> WaveTable::readScaleWavetable(float newMin, float newMax)
{
	std::vector<float> readWave = content;
	//We get the current min and max values of our read wavetable (not yet scaled)
	float currentMin = *std::min_element(readWave.begin(), readWave.end());
	float currentMax = *std::max_element(readWave.begin(), readWave.end());
	//We loop through the read wavetable
	for (int i = 0; i < readWave.size(); i++) {
		readWave.at(i) = scaleValue(readWave.at(i), currentMin, currentMax, newMin, newMax);
	}
	return readWave;
}

