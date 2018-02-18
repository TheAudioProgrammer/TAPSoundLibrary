/*
==============================================================================

Effect.h
Created: 15 Feb 2018 3:33:27pm
Author:  Johannes Wolfgruber

==============================================================================
*/
#include "BiQuadFilter.h"

tsl::audioeffects::BiQuadFilter::BiQuadFilter (double sampleRate,
								   			   int samplesPerBlock,
								   			   int FilterType,
								   			   double frequency,
								   			   double resonance)
	: tsl::audioeffects::Effect (sampleRate, samplesPerBlock)
{
	init(sampleRate, FilterType, frequency, resonance);
}

void tsl::audioeffects::BiQuadFilter::init (double sampleRate, 
									  		int FilterType,
									  		double frequency,
									  		double resonance)
{
	assert (sampleRate > 0);
	assert (frequency > 0.0 && frequency <= sampleRate * 0.5);
	assert (resonance > 0.0);

	this->sampleRate = sampleRate;
	switch (FilterType) {
		case firstOrderLP:
			setFirstOrderLP (frequency);
			break;
		case firstOrderHP:
			setFirstOrderHP (frequency);
			break;
		case secondOrderLP:
			setSecondOrderLP (frequency, resonance);
			break;
		case secondOrderHP:
			setSecondOrderHP (frequency, resonance);
			break;
		case secondOrderButterworthLP:
			setSecondOrderButterworthLP (frequency);
			break;
		case secondOrderButterworthHP:
			setSecondOrderButterworthHP (frequency);
			break;
		case secondOrderBP:
			setSecondOrderBP (frequency, resonance);
			break;
		case secondOrderBS:
			setSecondOrderBS (frequency, resonance);
			break;
		case secondOrderLinkwitzRileyLP:
			setSecondOrderLinkwitzRileyLP (frequency);
			break;
		case secondOrderLinkwitzRileyHP:
			setSecondOrderLinkwitzRileyHP (frequency);
			break;
		case firstOrderAP:
			setFirstOrderAP (frequency);
			break;
		case secondOrderAP:
			setSecondOrderAP (frequency, resonance);
			break;
		case secondOrderMassbergLP:
			setSecondOrderMassbergLP (frequency, resonance);
			break;
	}
}

void tsl::audioeffects::BiQuadFilter::reset()
{
	xm1 = xm2 = ym1 = ym2 = 0.0;
	xs1 = xs2 = ys1 = ys2 = 0.0;
}

void tsl::audioeffects::BiQuadFilter::setFirstOrderLP (double frequency) const
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);

	const double theta = 2.0 * M_PI * frequency / sampleRate;
	const double gamma = cos(theta) / (1.0 + sin(theta));
	a0 = (1.0 - gamma) / 2.0;
	a1 = a0;
	a2 = 0.0;
	b1 = -gamma;
	b2 = 0.0;
}

void tsl::audioeffects::BiQuadFilter::setFirstOrderHP (double frequency) const
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);

	const double theta = 2.0 * M_PI * frequency / sampleRate;
	const double gamma = cos(theta) / (1.0 + sin(theta));
	a0 = (1.0 + gamma) / 2.0;
	a1 = -a0;
	a2 = 0.0;
	b1 = -gamma;
	b2 = 0.0;
}

void tsl::audioeffects::BiQuadFilter::setSecondOrderLP (double frequency,
												        double resonance) const
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);
	assert(resonance > 0.0);

	const double theta = 2.0 * M_PI * frequency / sampleRate;
	const double delta = 1.0 / resonance;
	const double beta = 0.5 * (1.0 - 0.5 * delta * sin(theta))
				/ (1.0 + 0.5 * delta * sin(theta));
	const double gamma = (0.5 + beta) * cos(theta);
	const double tmp = 0.5 + beta - gamma;
	a0 = tmp / 2.0;
	a1 = tmp;
	a2 = a0;
	b1 = -2.0 * gamma;
	b2 = 2.0 * beta;
}

void tsl::audioeffects::BiQuadFilter::setSecondOrderHP (double frequency,
												        double resonance) const
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);
	assert(resonance > 0.0);

	const double theta = 2.0 * M_PI * frequency / sampleRate;
	const double delta = 1.0 / resonance;
	const double beta = 0.5 * (1.0 - 0.5 * delta * sin(theta))
				/ (1.0 + 0.5 * delta * sin(theta));
	const double gamma = (0.5 + beta) * cos(theta);
	const double tmp = 0.5 + beta + gamma;
	a0 = tmp / 2.0;
	a1 = -tmp;
	a2 = a0;
	b1 = -2.0 * gamma;
	b2 = 2.0 * beta;
}

void tsl::audioeffects::BiQuadFilter::setSecondOrderButterworthLP (double frequency) const
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);

	const double c = 1.0 / tan(M_PI * frequency / sampleRate);
	a0 = 1.0 / (1.0 + sqrt(2.0) * c + c * c);
	a1 = 2.0 * a0;
	a2 = a0;
	b1 = 2.0 * a0 * (1.0 - c * c);
	b2 = a0 * (1.0 - sqrt(2.0) * c + c * c);
}

void tsl::audioeffects::BiQuadFilter::setSecondOrderButterworthHP (double frequency) const
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);

	const double c = tan(M_PI * frequency / sampleRate);
	a0 = 1.0 / (1.0 + sqrt(2.0) * c + c * c);
	a1 = -2.0 * a0;
	a2 = a0;
	b1 = 2.0 * a0 * (c * c - 1.0);
	b2 = a0 * (1.0 - sqrt(2.0) * c + c * c);
}

void tsl::audioeffects::BiQuadFilter::setSecondOrderBP (double frequency,
												  		double resonance) const
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);
	assert(resonance > 0.0);

	const double theta = 2 * M_PI * frequency / sampleRate;
	const double tmp = theta / (2.0 * resonance);
	const double beta = 0.5 * (1.0 - tan(clamp(tmp, 0.0, M_PI_2 - 0.001)))
				/ (1.0 + tan(clamp(tmp, 0.0, M_PI_2 - 0.001)));
	const double gamma = (0.5 + beta) * cos(theta);
	a0 = 0.5 - beta;
	a1 = 0.0;
	a2 = -a0;
	b1 = -2 * gamma;
	b2 = 2 * beta;
}

void tsl::audioeffects::BiQuadFilter::setSecondOrderBS (double frequency,
												  		double resonance) const
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);
	assert(resonance > 0.0);

	const double theta = 2 * M_PI * frequency / sampleRate;
	const double tmp = theta / (2.0 * resonance);
	const double beta = 0.5 * (1.0 - tan(clamp(tmp, 0.0, M_PI_2 - 0.001)))
				/ (1.0 + tan(clamp(tmp, 0.0, M_PI_2 - 0.001)));
	const double gamma = (0.5 + beta) * cos(theta);
	a0 = 0.5 + beta;
	a1 = -2 * gamma;
	a2 = a0;
	b1 = a1;
	b2 = 2 * beta;
}

void tsl::audioeffects::BiQuadFilter::setSecondOrderLinkwitzRileyLP (double frequency) const
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);

	const double theta = M_PI * frequency / sampleRate;
	const double omega = M_PI * frequency;
	const double kappa = omega / tan(theta);
	const double delta = (kappa * kappa) + (omega * omega) + 2.0 * kappa * omega;
	a0 = (omega * omega / delta);
	a1 = 2.0 * a0;
	a2 = a0;
	b1 = ((-2.0 * kappa * kappa) + (2.0 * omega * omega)) / delta;
	b2 = ((-2.0 * kappa * omega) + (kappa * kappa) + (omega * omega)) / delta;
}

void tsl::audioeffects::BiQuadFilter::setSecondOrderLinkwitzRileyHP (double frequency) const
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);;

	const double theta = M_PI * frequency / sampleRate;
	const double omega = M_PI * frequency;
	const double kappa = omega / tan(theta);
	const double delta = (kappa * kappa) + (omega * omega) + 2.0 * kappa * omega;
	a0 = (kappa * kappa) / delta;
	a1 = -2.0 * a0;
	a2 = a0;
	b1 = ((-2.0 * kappa * kappa) + (2.0 * omega * omega)) / delta;
	b2 = ((-2.0 * kappa * omega) + (kappa * kappa) + (omega * omega)) / delta;
}

void tsl::audioeffects::BiQuadFilter::setFirstOrderAP (double frequency) const
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);

	const double alpha = (tan(M_PI * frequency / sampleRate) - 1.0)
				 / (tan(M_PI * frequency / sampleRate) + 1.0);

	a0 = alpha;
	a1 = 1.0;
	a2 = 0.0;
	b1 = alpha;
	b2 = 0.0;
}

void tsl::audioeffects::BiQuadFilter::setSecondOrderAP (double frequency,
												  		double resonance) const
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);
	assert(resonance > 0.0);

	const double alpha = (tan(M_PI * resonance / sampleRate) - 1.0)
				 / (tan(M_PI * resonance / sampleRate) + 1.0);
	const double theta = M_PI * frequency / sampleRate;
	const double beta = -cos(theta);

	a0 = -alpha;
	a1 = beta * (1.0 - alpha);
	a2 = 1.0;
	b1 = a1;
	b2 = -alpha;
}

void tsl::audioeffects::BiQuadFilter::setSecondOrderMassbergLP (double frequency,
														  		double resonance) const
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);
	assert(resonance > 0.0);
	
	const double theta = 2 * M_PI * frequency / sampleRate;
	const double tmp1 = M_PI / theta;
	const double tmp2 = M_PI / (resonance * theta);
	const double g1 = 2 / sqrt ((1 - tmp1 * tmp1) * (1 - tmp1 * tmp1) + (tmp2 * tmp2));
	const double res2 = resonance * resonance;
	double gr, wr, omegaR, omegaS, wm, omegaM;
	
	if (resonance <= sqrt (0.5))
	{
		wm = theta * sqrt ((1 - 1 / (2 * res2) + sqrt ((1 - 4 * res2) / (res2 * res2) + 1 / g1)));
		omegaM = tan (wm / 2);
		omegaS = (0.5 * theta * pow (1 - g1 * g1, 0.25));
		omegaS = std::min (omegaS, omegaM);
	}
	else if (resonance > sqrt (0.5))
	{
		gr = (2 * res2) / sqrt (4 * res2 - 1);
		wr = theta * sqrt (1 - 1 / (2 * res2));
		omegaR = tan (wr / 2);
		omegaS = omegaR * pow ((gr * gr - g1 * g1) / (gr * gr - 1), 0.25);
	}
	
	const double wp = 2 * atan (omegaS);
	const double wz = 2 * atan (omegaS / sqrt (g1));
	const double tmp3 = wp / theta;
	const double tmp4 = wp / (resonance * theta);
	const double tmp5 = wz / theta;
	const double tmp6 = wz / (resonance * theta);
	const double tmp32 = tmp3 * tmp3;
	const double tmp52 = tmp5 * tmp5;
	const double gp = 1 / sqrt ((1 - tmp32) * (1 - tmp32) + (tmp4 * tmp4));
	const double gz = 1 / sqrt ((1 - tmp52) * (1 - tmp52) + (tmp6 * tmp6));
	const double tmp7 = g1 * (gp * gp - gz * gz);
	const double tmp8 = (g1 - 1) * (g1 - 1);
	const double gz2 = gz * gz;
	const double Qp = sqrt (tmp7 / (tmp8 * (g1 + gz2)));
	const double Qz = sqrt ((tmp7 * g1) / (gz2 * (g1 + gp * gp) * tmp8));
	const double omegaS2 = omegaS * omegaS;
	const double gamma0 = omegaS2 + omegaS / Qp + 1;
	const double alpha0 = omegaS2 + (sqrt (g1) / Qz) * omegaS + g1;
	const double alpha1 = 2 * (omegaS2 - g1);
	const double alpha2 = omegaS2 - (sqrt (g1) / Qz) * omegaS + g1;
	const double beta1 = 2 * (omegaS2 - 1);
	const double beta2 = omegaS2 - omegaS / Qp + 1;
	
	a0 = alpha0 / gamma0;
	a1 = alpha1 / gamma0;
	a2 = alpha2 / gamma0;
	b1 = beta1 / gamma0;
	b2 = beta2 / gamma0;
}

double tsl::audioeffects::BiQuadFilter::clamp (double x, double lower, double upper) const
{
	return std::min(upper, std::max(x, lower));
}

void tsl::audioeffects::BiQuadFilter::setSampleRate (double sampleRate)
{
	this->sampleRate = sampleRate;
}

double tsl::audioeffects::BiQuadFilter::getSampleRate() const
{
	return this->sampleRate;
}

void tsl::audioeffects::BiQuadFilter::setSamplesPerBlock (int samplesPerBlock)
{
	this->samplesPerBlock = samplesPerBlock;
}

int tsl::audioeffects::BiQuadFilter::getSamplesPerBlock() const
{
	return this->samplesPerBlock;
}

void tsl::audioeffects::BiQuadFilter::process (float* left, float* right, int numSamples)
{
	for (int i = 0; i < numSamples; ++i)
	{
		const double inputL = left[i];
		const double inputR = right[i];
		const double outL = a0 * inputL + a1 * xm1 + a2 * xm2 - b1 * ym1 - b2 * ym2;;
		const double outR = a0 * inputR + a1 * xs1 + a2 * xs2 - b1 * ys1 - b2 * ys2;;
		
		xm2 = xm1;
		xs2 = xs1;
		xm1 = inputL;
		xs1 = inputR;
		ym2 = ym1;
		ys2 = ys1;
		ym1 = outL;
		ys1 = outR;
		
		left[i] = outL;
		right[i] = outR;
	}
}

void tsl::audioeffects::BiQuadFilter::process (float* mono, int numSamples)
{
	for (int i = 0; i < numSamples; ++i)
	{
		const double input = mono[i];
		const double out = a0 * input + a1 * xm1 + a2 * xm2 - b1 * ym1 - b2 * ym2;
		
		xm2 = xm1;
		xm1 = input;
		ym2 = ym1;
		ym1 = out;
		
		mono[i] = out;
	}
}

float tsl::audioeffects::BiQuadFilter::processSample (float input)
{
	const double out = a0 * input + a1 * xm1 + a2 * xm2 - b1 * ym1 - b2 * ym2;
	
	xm2 = xm1;
	xm1 = input;
	ym2 = ym1;
	ym1 = out;
	
	return out;
}

