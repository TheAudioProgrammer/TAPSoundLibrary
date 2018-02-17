/*
==============================================================================

Effect.h
Created: 15 Feb 2018 3:33:27pm
Author:  Johannes Wolfgruber

==============================================================================
*/
#include "Filter.h"

tsl::audioeffects::Filter::Filter (double sampleRate,
							  int samplesPerBlock,
							  int FilterType,
							  double frequency,
							  double resonance)
	: tsl::audioeffects::Effect (sampleRate, samplesPerBlock)
{
	init(sampleRate, FilterType, frequency, resonance);
}

void tsl::audioeffects::Filter::init (double sampleRate, 
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
	}
}

void tsl::audioeffects::Filter::reset()
{
	xm1 = xm2 = ym1 = ym2 = 0.0;
	xs1 = xs2 = ys1 = ys2 = 0.0;
}

void tsl::audioeffects::Filter::setFirstOrderLP (double frequency)
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);

	auto theta = 2.0 * M_PI * frequency / sampleRate;
	auto gamma = cos(theta) / (1.0 + sin(theta));
	a0 = (1.0 - gamma) / 2.0;
	a1 = a0;
	a2 = 0.0;
	b1 = -gamma;
	b2 = 0.0;
}

void tsl::audioeffects::Filter::setFirstOrderHP (double frequency)
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);

	auto theta = 2.0 * M_PI * frequency / sampleRate;
	auto gamma = cos(theta) / (1.0 + sin(theta));
	a0 = (1.0 + gamma) / 2.0;
	a1 = -a0;
	a2 = 0.0;
	b1 = -gamma;
	b2 = 0.0;
}

void tsl::audioeffects::Filter::setSecondOrderLP (double frequency,
											 double resonance)
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);
	assert(resonance > 0.0);

	auto theta = 2.0 * M_PI * frequency / sampleRate;
	auto delta = 1.0 / resonance;
	auto beta = 0.5 * (1.0 - 0.5 * delta * sin(theta))
				/ (1.0 + 0.5 * delta * sin(theta));
	auto gamma = (0.5 + beta) * cos(theta);
	auto tmp = 0.5 + beta - gamma;
	a0 = tmp / 2.0;
	a1 = tmp;
	a2 = a0;
	b1 = -2.0 * gamma;
	b2 = 2.0 * beta;
}

void tsl::audioeffects::Filter::setSecondOrderHP (double frequency,
											 double resonance)
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);
	assert(resonance > 0.0);

	auto theta = 2.0 * M_PI * frequency / sampleRate;
	auto delta = 1.0 / resonance;
	auto beta = 0.5 * (1.0 - 0.5 * delta * sin(theta))
				/ (1.0 + 0.5 * delta * sin(theta));
	auto gamma = (0.5 + beta) * cos(theta);
	auto tmp = 0.5 + beta + gamma;
	a0 = tmp / 2.0;
	a1 = -tmp;
	a2 = a0;
	b1 = -2.0 * gamma;
	b2 = 2.0 * beta;
}

void tsl::audioeffects::Filter::setSecondOrderButterworthLP (double frequency)
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);

	auto c = 1.0 / tan(M_PI * frequency / sampleRate);
	a0 = 1.0 / (1.0 + sqrt(2.0) * c + c * c);
	a1 = 2.0 * a0;
	a2 = a0;
	b1 = 2.0 * a0 * (1.0 - c * c);
	b2 = a0 * (1.0 - sqrt(2.0) * c + c * c);
}

void tsl::audioeffects::Filter::setSecondOrderButterworthHP (double frequency)
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);

	auto c = tan(M_PI * frequency / sampleRate);
	a0 = 1.0 / (1.0 + sqrt(2.0) * c + c * c);
	a1 = -2.0 * a0;
	a2 = a0;
	b1 = 2.0 * a0 * (c * c - 1.0);
	b2 = a0 * (1.0 - sqrt(2.0) * c + c * c);
}

void tsl::audioeffects::Filter::setSecondOrderBP (double frequency,
											 double resonance)
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);
	assert(resonance > 0.0);

	auto theta = 2 * M_PI * frequency / sampleRate;
	auto tmp = theta / (2.0 * resonance);
	auto beta = 0.5 * (1.0 - tan(clamp(tmp, 0.0, M_PI_2 - 0.001)))
				/ (1.0 + tan(clamp(tmp, 0.0, M_PI_2 - 0.001)));
	auto gamma = (0.5 + beta) * cos(theta);
	a0 = 0.5 - beta;
	a1 = 0.0;
	a2 = -a0;
	b1 = -2 * gamma;
	b2 = 2 * beta;
}

void tsl::audioeffects::Filter::setSecondOrderBS (double frequency,
											 double resonance)
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);
	assert(resonance > 0.0);

	auto theta = 2 * M_PI * frequency / sampleRate;
	auto tmp = theta / (2.0 * resonance);
	auto beta = 0.5 * (1.0 - tan(clamp(tmp, 0.0, M_PI_2 - 0.001)))
				/ (1.0 + tan(clamp(tmp, 0.0, M_PI_2 - 0.001)));
	auto gamma = (0.5 + beta) * cos(theta);
	a0 = 0.5 + beta;
	a1 = -2 * gamma;
	a2 = a0;
	b1 = a1;
	b2 = 2 * beta;
}

void tsl::audioeffects::Filter::setSecondOrderLinkwitzRileyLP (double frequency)
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);

	auto theta = M_PI * frequency / sampleRate;
	auto omega = M_PI * frequency;
	auto kappa = omega / tan(theta);
	auto delta = (kappa * kappa) + (omega * omega) + 2.0 * kappa * omega;
	a0 = (omega * omega / delta);
	a1 = 2.0 * a0;
	a2 = a0;
	b1 = ((-2.0 * kappa * kappa) + (2.0 * omega * omega)) / delta;
	b2 = ((-2.0 * kappa * omega) + (kappa * kappa) + (omega * omega)) / delta;
}

void tsl::audioeffects::Filter::setSecondOrderLinkwitzRileyHP (double frequency)
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);;

	auto theta = M_PI * frequency / sampleRate;
	auto omega = M_PI * frequency;
	auto kappa = omega / tan(theta);
	auto delta = (kappa * kappa) + (omega * omega) + 2.0 * kappa * omega;
	a0 = (kappa * kappa) / delta;
	a1 = -2.0 * a0;
	a2 = a0;
	b1 = ((-2.0 * kappa * kappa) + (2.0 * omega * omega)) / delta;
	b2 = ((-2.0 * kappa * omega) + (kappa * kappa) + (omega * omega)) / delta;
}

void tsl::audioeffects::Filter::setFirstOrderAP (double frequency)
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);

	auto alpha = (tan(M_PI * frequency / sampleRate) - 1.0)
				 / (tan(M_PI * frequency / sampleRate) + 1.0);

	a0 = alpha;
	a1 = 1.0;
	a2 = 0.0;
	b1 = alpha;
	b2 = 0.0;
}

void tsl::audioeffects::Filter::setSecondOrderAP (double frequency,
											 double resonance)
{
	assert(sampleRate > 0);
	assert(frequency > 0.0 && frequency <= sampleRate * 0.5);
	assert(resonance > 0.0);

	auto alpha = (tan(M_PI * resonance / sampleRate) - 1.0)
				 / (tan(M_PI * resonance / sampleRate) + 1.0);
	auto theta = M_PI * frequency / sampleRate;
	auto beta = -cos(theta);

	a0 = -alpha;
	a1 = beta * (1.0 - alpha);
	a2 = 1.0;
	b1 = a1;
	b2 = -alpha;
}

double tsl::audioeffects::Filter::clamp (double x, double lower, double upper)
{
	return std::min(upper, std::max(x, lower));
}

void tsl::audioeffects::Filter::setSampleRate (double sampleRate)
{
	this->sampleRate = sampleRate;
}

double tsl::audioeffects::Filter::getSampleRate()
{
	return this->sampleRate;
}

void tsl::audioeffects::Filter::setSamplesPerBlock (int samplesPerBlock)
{
	this->samplesPerBlock = samplesPerBlock;
}

int tsl::audioeffects::Filter::getSamplesPerBlock()
{
	return this->samplesPerBlock;
}

void tsl::audioeffects::Filter::process (float* left, float* right, int numSamples)
{
	for (int i = 0; i < numSamples; ++i)
	{
		auto inputL = left[i];
		auto inputR = right[i];
		auto outL = a0 * inputL + a1 * xm1 + a2 * xm2 - b1 * ym1 - b2 * ym2;;
		auto outR = a0 * inputR + a1 * xs1 + a2 * xs2 - b1 * ys1 - b2 * ys2;;
		
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

void tsl::audioeffects::Filter::process (float* mono, int numSamples)
{
	for (int i = 0; i < numSamples; ++i)
	{
		auto input = mono[i];
		auto out = a0 * input + a1 * xm1 + a2 * xm2 - b1 * ym1 - b2 * ym2;
		
		xm2 = xm1;
		xm1 = input;
		ym2 = ym1;
		ym1 = out;
		
		mono[i] = out;
	}
}

float tsl::audioeffects::Filter::processSample (float input)
{
	auto out = a0 * input + a1 * xm1 + a2 * xm2 - b1 * ym1 - b2 * ym2;
	
	xm2 = xm1;
	xm1 = input;
	ym2 = ym1;
	ym1 = out;
	
	return out;
}
