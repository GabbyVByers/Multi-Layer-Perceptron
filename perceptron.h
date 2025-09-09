#pragma once

#include "dataset.h"
#include "profiler.h"
#include <vector>

class Perceptron
{
public:

	const std::vector<int> networkStructure = { 784, 32, 32, 32, 32, 10 };
	DataSet* dataset;
	int indexCurrTrainingExample = 0;
	float learningRate = 0.01f;
	float currentCost = 0.0f;
	bool doContinuousTraining = true;

	float*** weights        = nullptr;
	float**  biases         = nullptr;
	float**  activations    = nullptr;
	float**  z_values       = nullptr;
	float**  dCdz           = nullptr;
	float*** dCdw           = nullptr;
	float*   expectedValues = nullptr;

	Profiler profiler;
	double trainingTime = 0.0;

	// perceptron_memory.cpp
	Perceptron();
	~Perceptron();
	void mallocNetwork();
	void freeNetwork();
	void initWeightsAndBiasesWithRandomValues();
	int getCurrExpectedValue();

	// perceptron_propagate.cpp
	void putThatClankerToWork();
	void mapTrainingExampleToInputLayer();
	void propagateForwards();
	void propagateBackwards();
	void updateWeightsAndBiases();
	void trackTotalCost();
};

