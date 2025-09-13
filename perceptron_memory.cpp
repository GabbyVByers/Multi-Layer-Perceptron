
#include "perceptron.h"
#include "random.h"

Perceptron::Perceptron()
{
	trainingDataset = new DataSet("training");
	testDataset = new DataSet("testing");

	int numLayers = networkStructure.size();
	for (int L = 1; L < numLayers; L++)
	{
		numParameters += networkStructure[L];
		numParameters += networkStructure[L] * networkStructure[L - 1];
	}

	mallocNetwork();
	initWeightsWithRandomValues();
}

Perceptron::~Perceptron()
{
	freeNetwork();
	delete trainingDataset;
	delete testDataset;
}

void Perceptron::mallocNetwork()
{
	int numLayers = networkStructure.size();

	biases = new float* [numLayers];
	activations = new float* [numLayers];
	z_values = new float* [numLayers];
	dCdz = new float* [numLayers];
	weights = new float** [numLayers];
	dCdw = new float** [numLayers];
	expectedValues = new float[networkStructure[numLayers - 1]]();

	for (int L = 1; L < numLayers; L++)
	{
		int J = networkStructure[L];
		int K = networkStructure[L - 1];
		weights[L] = new float* [J];
		dCdw[L] = new float* [J];
		for (int j = 0; j < J; j++)
		{
			weights[L][j] = new float[K]();
			dCdw[L][j] = new float[K]();
		}

		biases[L] = new float[J]();
		z_values[L] = new float[J]();
		dCdz[L] = new float[J]();
	}

	for (int L = 0; L < numLayers; L++)
	{
		int J = networkStructure[L];
		activations[L] = new float[J]();
	}
}

void Perceptron::freeNetwork()
{
	int numLayers = networkStructure.size();

	for (int L = 1; L < numLayers; L++)
	{
		int J = networkStructure[L];
		for (int j = 0; j < J; j++)
		{
			delete[] weights[L][j];
			delete[] dCdw[L][j];
		}

		delete[] weights[L];
		delete[] dCdw[L];
		delete[] biases[L];
		delete[] z_values[L];
		delete[] dCdz[L];
	}
	delete[] weights;
	delete[] dCdw;

	for (int L = 0; L < numLayers; L++)
	{
		delete[] activations[L];
	}

	delete[] biases;
	delete[] activations;
	delete[] z_values;
	delete[] dCdz;
	delete[] expectedValues;
}

inline static float randf()
{
	return randomFloat(-1.0f, 1.0f);
}

void Perceptron::initWeightsWithRandomValues()
{
	int numLayers = networkStructure.size();

	for (int L = 1; L < numLayers; L++)
	{
		int J = networkStructure[L];
		int K = networkStructure[L - 1];

		for (int j = 0; j < J; j++)
		{
			for (int k = 0; k < K; k++)
			{
				weights[L][j][k] = randf() * 0.5f;
			}
		}
	}
}

int Perceptron::getCurrExpectedValue()
{
	if (isBeingBenchmarked)
	{
		if (indexCurrTestExample == -1) return -1;
		return testDataset->handWrittenDigits[indexCurrTestExample].trueValue;
	}
	else
	{
		if (indexCurrTrainingExample == -1) return -1;
		return trainingDataset->handWrittenDigits[indexCurrTrainingExample].trueValue;
	}
	
}

