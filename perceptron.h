#pragma once

#include "dataset.h"
#include "profiler.h"
#include <vector>

class Perceptron
{
public:

	const std::vector<int> networkStructure = { 784, 32, 32, 32, 10 };
	DataSet* dataset;
	int indexCurrTrainingExample = 0;
	float learningRate = 0.01f;
	bool isContinuouslyTraining = true;
	int networkChoice = -1;
	int trainingExamplesPerFrame = 128;
	
	float currentCost = 0.0f;
	std::vector<float> longRunCostHistory;

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
	void initWeightsWithRandomValues();
	int getCurrExpectedValue();

	// perceptron_propagate.cpp
	void continuouslyTrain();
	void trainOnASingleExample(int direction);
	void mapTrainingExampleToInputLayer();
	void propagateForwards();
	void propagateBackwards();
	void updateWeightsAndBiases();

	// perceptron_history.cpp
	void trackTotalCost();
	void trackCostHistory();
	void trackAccuracyHistory();
};

