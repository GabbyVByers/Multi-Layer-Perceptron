#pragma once

#include <cmath>

inline float sigmoid(float x)
{
	float ex = std::pow(2.7182817f, x);
	return ex / (1 + ex);
}

inline float sigmoidDerivative(float x)
{
	float ex = std::pow(2.7182817f, x);
	return ex / ((1 + ex) * (1 + ex));
}

