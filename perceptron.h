#pragma once

#include <vector>

class Perceptron
{
public:

	int numLayers = -1;
	const std::vector<int> networkStructure = { 784, 16, 16, 10 };

	float*** weights = nullptr;
	float** biases = nullptr;
	float** activations = nullptr;
	float** z_values = nullptr;
	float** dCdz = nullptr;
	float*** dCdw = nullptr;
	float* expectedValues = nullptr;

	Perceptron()
	{
		numLayers = networkStructure.size();
		mallocNetwork();
	}

	~Perceptron()
	{
		freeNetwork();
	}

	void mallocNetwork();
	void freeNetwork();
};

