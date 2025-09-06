#pragma once

#include <vector>

class Perceptron
{
public:

	const std::vector<int> networkStructure = { 784, 16, 16, 10 };

	float* inputActivations = nullptr;
	float** networkActivations = nullptr;
	float** networkBises = nullptr;
	float*** networkWeights = nullptr;

	Perceptron()
	{

	}
};