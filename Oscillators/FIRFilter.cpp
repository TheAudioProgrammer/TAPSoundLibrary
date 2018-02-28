//
//  FIRFilter.cpp
//  FIR_Object - App
//
//  Created by Samuel Thompson Parke-Wolfe on 22/02/2018.
//

#include "FIRFilter.h"

namespace tsl
{
    namespace audioeffects
    {
        
        const int FIRFilter::ir_max_length = 512;
        
        FIRFilter::FIRFilter (double sampleRate, int samplesPerBlock)
        :
        Effect (sampleRate, samplesPerBlock)
        {
            
            //Create new array for the impulse responce.
            ir = new float[ir_max_length];
            
            //Create new array for the output accumulator.
            output_accum = new float[(ir_max_length - 1) + samplesPerBlock];
            
            //Zero the output accumulator.
            memset(output_accum, 0, sizeof(*output_accum));
            
            //Initialise IR to a bandpass filter.
            float bandpass_ir [ FIRFilter::ir_max_length] =
            {
                -0.0448093,  0.0322875,   0.0181163,   0.0087615,   0.0056797,
                0.0086685,  0.0148049,   0.0187190,   0.0151019,   0.0027594,
                -0.0132676, -0.0232561,  -0.0187804,   0.0006382,   0.0250536,
                0.0387214,  0.0299817,   0.0002609,  -0.0345546,  -0.0525282,
                -0.0395620,  0.0000246,   0.0440998,   0.0651867,   0.0479110,
                0.0000135, -0.0508558,  -0.0736313,  -0.0529380,  -0.0000709,
                0.0540186,  0.0766746,   0.0540186,  -0.0000709,  -0.0529380,
                -0.0736313, -0.0508558,   0.0000135,   0.0479110,   0.0651867,
                0.0440998,  0.0000246,  -0.0395620,  -0.0525282,  -0.0345546,
                0.0002609,  0.0299817,   0.0387214,   0.0250536,   0.0006382,
                -0.0187804, -0.0232561,  -0.0132676,   0.0027594,   0.0151019,
                0.0187190,  0.0148049,   0.0086685,   0.0056797,   0.0087615,
                0.0181163,  0.0322875,  -0.0448093
            };
            irlen = 63;
            
            memcpy(ir, bandpass_ir, ir_max_length*sizeof(float));
            
        }
        
        
        void FIRFilter::process (float *mono, int numSamples)
        {
            float acc;  // accumulator for MACs
            float *irp; // pointer to coefficients
            float *inputp; // pointer to input samples
            int n, k;
            
            memcpy( &output_accum[irlen - 1], mono,
                   numSamples * sizeof(float) );
            
            // apply the filter to each input sample
            for ( n = 0; n < numSamples; n++ )
            {
                
                irp = ir;
                inputp = &output_accum[irlen - 1 + n];
                acc = 0;
                
                for ( k = 0; k < irlen; k++ ) {
                    acc += (*irp++) * (*inputp--);
                }
                
                mono[n] = acc;
            }
            
            // shift input samples back in time for next round
            memmove( &output_accum[0], &output_accum[numSamples],
                    (irlen - 1) * sizeof(float) );
        }
        
        
        void FIRFilter::process (float *left, float *right, int numSamples)
        {
            /* Yet to be implemented */
            /* For now just use one object per channel */
        }
        
        void FIRFilter::setIr (long irlen, float *ir)
        {
            //The length of the impulse responce must be larger than 0 and smaller than MAX_IR_LEN.
            assert(irlen > 0 && irlen <= FIRFilter::ir_max_length);
            
            this->irlen = irlen;
            for(int i = 0; i<irlen; ++i)
            {
                this->ir[i] = ir[i];
            }
        }
        
        float* FIRFilter::getIr ()
        {
            return this->ir;
        }
        
        long FIRFilter::getIrlen ()
        {
            return this->irlen;
        }
        
    }
}
