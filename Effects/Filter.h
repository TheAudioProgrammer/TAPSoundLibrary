/*
==============================================================================

Effect.h
Created: 15 Feb 2018 3:33:27pm
Author:  Johannes Wolfgruber

==============================================================================
*/

#pragma once

#include "Effect.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>
#include <algorithm>

namespace tsl {

	namespace audioeffects

	{
		class Filter : public Effect
		{
		public:
			enum {							// filter types
				firstOrderLP,
				firstOrderHP,
				secondOrderLP,
				secondOrderHP,
				secondOrderButterworthLP,
				secondOrderButterworthHP,
				secondOrderBP,
				secondOrderBS,
				secondOrderLinkwitzRileyLP,
				secondOrderLinkwitzRileyHP,
				firstOrderAP,
				secondOrderAP
			};

			Filter (double sampleRate,
					int samplesPerBlock,
					int FilterType,
					double frequency = 5000.0,
					double resonance = 0.707);

			void init (double sampleRate,
					   int FilterType,
					   double frequency = 5000.0,
					   double resonance = 0.707);
			
			void reset();

			void setFirstOrderLP (double frequency);

			void setFirstOrderHP (double frequency);

			void setSecondOrderLP (double frequency,
								   double resonance);

			void setSecondOrderHP (double frequency,
								   double resonance);

			void setSecondOrderButterworthLP (double frequency);

			void setSecondOrderButterworthHP (double frequency);

			void setSecondOrderBP (double frequency,
								   double resonance);

			void setSecondOrderBS (double frequency,
								   double resonance);

			void setSecondOrderLinkwitzRileyLP (double frequency);

			void setSecondOrderLinkwitzRileyHP (double frequency);

			void setFirstOrderAP (double frequency);

			void setSecondOrderAP (double frequency,
								   double resonance);
			
			double clamp(double x, double lower, double upper);

			void setSampleRate (double sampleRate);
			double getSampleRate();

			void setSamplesPerBlock (int samplesPerBlock);
			int getSamplesPerBlock();

			void process (float* left, float* right, int numSamples);
			void process (float* mono, int numSamples);

			float processSample (float sample);
		protected:
			double a0, a1, a2, b1, b2;					// filter coefficients
			double xm1 = 0, xm2 = 0, ym1 = 0, ym2 = 0;	// previous samples
			double xs1 = 0, xs2 = 0, ys1 = 0, ys2 = 0; 	// additional previous samples for stereo process method
		};
	}
}
