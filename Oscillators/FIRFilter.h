//
//  FIRFilter.h
//  FIR_Object - App
//
//  Created by Samuel Thompson Parke-Wolfe on 22/02/2018.
//

#ifndef FIRFilter_h
#define FIRFilter_h

#include "Effect.h"

#include <stdio.h>
#include <assert.h>
#include <cstring>

#pragma once

//Thanks to https://sestevenson.wordpress.com/implementation-of-fir-filtering-in-c-part-1/ for writing this algorithm.

namespace tsl
{
    namespace audioeffects
    {
        
        class FIRFilter : public Effect
        {
        public:
            FIRFilter(double sampleRate, int samplesPerBlock);
            
            void process (float* mono, int numSamples);
            void process (float* left, float* right, int numSamples);
            void setIr (long argc, float *argv);
            float* getIr ();
            long getIrlen ();

            static const int ir_max_length;
            
        private:
            
            float *ir;
            long irlen;
            float *output_accum;
        };
    }
}


#endif /* FIRFilter_h */
