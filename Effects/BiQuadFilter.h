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
		class BiQuadFilter : public Effect
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
				secondOrderAP,
				secondOrderMassbergLP
			};

			BiQuadFilter (double sampleRate,
						  int samplesPerBlock,
						  int FilterType,
						  double frequency = 5000.0,
						  double resonance = 0.707);

			void init (double sampleRate,
					   int FilterType,
					   double frequency = 5000.0,
					   double resonance = 0.707);
			
			void reset();

			void setFirstOrderLP (double frequency) const;

			void setFirstOrderHP (double frequency) const;

			void setSecondOrderLP (double frequency,
								   double resonance) const;

			void setSecondOrderHP (double frequency,
								   double resonance) const;

			void setSecondOrderButterworthLP (double frequency) const;

			void setSecondOrderButterworthHP (double frequency) const;

			void setSecondOrderBP (double frequency,
								   double resonance) const;

			void setSecondOrderBS (double frequency,
								   double resonance) const;

			void setSecondOrderLinkwitzRileyLP (double frequency) const;

			void setSecondOrderLinkwitzRileyHP (double frequency) const;

			void setFirstOrderAP (double frequency) const;

			void setSecondOrderAP (double frequency,
								   double resonance) const;
			
			void setSecondOrderMassbergLP (double frequency,
										   double resonance) const;
			
			double clamp(double x, double lower, double upper) const;

			void setSampleRate (double sampleRate);
			double getSampleRate() const;

			void setSamplesPerBlock (int samplesPerBlock);
			int getSamplesPerBlock() const;

			void process (float* left, float* right, int numSamples);
			void process (float* mono, int numSamples);

			float processSample (float sample);
		protected:
			mutable double a0, a1, a2, b1, b2;					// filter coefficients
			double xm1 = 0, xm2 = 0, ym1 = 0, ym2 = 0;	// previous samples
			double xs1 = 0, xs2 = 0, ys1 = 0, ys2 = 0; 	// additional previous samples for stereo process method
		};
	}
}
