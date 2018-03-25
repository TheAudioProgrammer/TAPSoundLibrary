/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.txt file.


 BEGIN_JUCE_MODULE_DECLARATION

 ID:               tapsoundlibrary
 vendor:           theaudioprogrammer
 version:          1.0.0
 name:             JUCE audio and MIDI data classes
 description:      Effects and utility classes for JUCE and DSP
 website:          https://github.com/theaudioprogrammer/TAPSoundLibrary
 license:          ISC

 dependencies:     juce_core
 OSXFrameworks:    Accelerate
 iOSFrameworks:    Accelerate

 END_JUCE_MODULE_DECLARATION

 *******************************************************************************/


#include "Oscillators/WaveTable.h"
#include "Oscillators/Oscillator.h"
#include "Effects/Effect.h"
#include "Effects/Gain.h"
#include "Effects/BiQuadFilter.h"
#include "Effects/FIRFilter.h"
