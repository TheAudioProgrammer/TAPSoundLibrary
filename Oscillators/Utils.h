/*
  ==============================================================================

    Utils.h
    Created: 15 Feb 2018 8:46:50pm
    Author:  Claudio

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

#pragma once

class Utils
{
	public:
	//We can calculate our PI that we will use in ensureValueZeroTwoPI
	const float PI = acos(-1.0);
	const float TwoPI = PI * 2.0;

	/*
	This method simply takes a value, the current min/max values and the new min/max values
	Here is an example
	We have a vector of 100 elements between 0.0 and 100.0
	We call(50.0, 0.0, 100.0, 0.0, 200.0)
	Our returned value will be 100
	This method reads the currentMin and currentMax that we have passed and it scale the value to fit to the newMin and newMax
	Another example
	We have a vector of 10 elements between -5.0 and 5.0
	We call(5.0, -5.0, 5.0, 10.0, 20.0)
	Our returned value will be 20.0
	*/
	static float scaleValue(float value, float currentMin, float currentMax, float newMin, float newMax) {
		return (newMax - newMin) * (value - currentMin) / (currentMax - currentMin) + newMin;
	}

	static float ensureValueZeroTwoPI(float value, float forMin, float forMax) {
		return Utils::scaleValue(value, forMin, forMax, 0.0, acos(-1.0) * 2.0);
	}

	static void debugFloat(String message, float value) {
		String strValue = String(value);
		String str = message;
		str += " : ";
		str += strValue;
		Logger::outputDebugString(str);
	}

	static void debugInt(String message, int value) {
		String strValue = String(value);
		String str = message;
		str += " : ";
		str += strValue;
		Logger::outputDebugString(str);
	}
};
