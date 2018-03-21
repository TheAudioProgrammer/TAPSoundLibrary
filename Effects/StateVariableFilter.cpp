/*
 ==============================================================================
 
 StateVariableFilter.cpp
 Created: 21 Mar 2018
 Author:  Nikolaj Andersson
State-Variable filter model from The Art of VA filter design by Vadim Zavalishin

 ==============================================================================
 */

#include "StateVariableFilter.h"
#include <math.h>       /* tan */


void StateVariableFilter::setSampleRate(float samplingRate)
{
    T = 1.0/samplingRate;
}

void StateVariableFilter::resetFilter()
{
    yHP = 0.0f;
    yBP = 0.0f;
    yLP = 0.0f;

    state1 = 0.0f;
    state2 = 0.0f;
}

void StateVariableFilter::setCutoff(float cutoffInHz)
{
    float wd = cutoffInHz * 2 * M_PI;
    float wa = (2 / T) * tan(wd * T / 2);
    g = wa * T / 2;  //  Calculate g (gain element of integrator)
    cutoff = 1 / (1 + 2 * R * g + g * g);
}

void StateVariableFilter::setResonance(float resonance)
{
    R = resonance;
}

void StateVariableFilter::process(float input)
{
    // calculate output;
    yHP = cutoff * (input - (2 * R + g) * state1 - state2); 
    yBP = yHP * g + state1;
    yLP = yBP * g + state2;
            
    // update states
    state1 = yHP * g + yBP;
    state2 = yBP * g + yLP;
}

float StateVariableFilter::getLP()
{
    return yLP;
}
float StateVariableFilter::getBP()
{
    return yBP;
}
float StateVariableFilter::getHP()
{
    return yHP;
}
