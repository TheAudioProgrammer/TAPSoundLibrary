/*
  ==============================================================================

  StateVariableFilter.h
  Created: 21 Mar 2018
  Author:  Nikolaj Andersson
  State-Variable filter model from The Art of VA filter design by Vadim Zavalishin
  ==============================================================================
*/


class StateVariableFilter
{
public:
    void setSampleRate(float samplingRate);
    void process(float input);
    void resetFilter();
    void setCutoff(float cutoffInHz);
    void setResonance(float resonance);
    float getLP();
    float getBP();
    float getHP();

    
private:

  float yHP = 0; // Highpass
  float yBP = 0; // Bandpass
  float yLP = 0; // Lowpass

  float cutoff = 1000; 

  float state1 = 0;
  float state2 = 0;
        
  float T = 0;
  float R = 0.5;
  float g = 0;
        
};

