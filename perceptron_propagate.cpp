
#include "perceptron.h"
#include "sigmoid.h"

void Perceptron::putThatClankerToWork()
{
	profiler.start();
	indexCurrTrainingExample++;
	indexCurrTrainingExample %= dataset->handWrittenDigits.size();
	mapTrainingExampleToInputLayer();
	propagateForwards();
	trackTotalCost();
	propagateBackwards();
	updateWeightsAndBiases();
	profiler.stop();
	trainingTime = profiler.time();
}

void Perceptron::mapTrainingExampleToInputLayer()
{
	HandWrittenDigit trainingExample = dataset->handWrittenDigits[indexCurrTrainingExample];
	int trueValue = trainingExample.trueValue;

	for (int i = 0; i < 10; i++)
	{
		expectedValues[i] = 0.0f;
	}
	expectedValues[trueValue] = 1.0f;

	for (int j = 0; j < 784; j++)
	{
		activations[0][j] = trainingExample.pixels[j];
	}
}

void Perceptron::propagateForwards()
{
	int numLayers = networkStructure.size();

	for (int L = 1; L < numLayers; L++)
	{
		int K = networkStructure[L - 1];
		int J = networkStructure[L];
		
		for (int j = 0; j < J; j++)
		{
			z_values[L][j] = 0.0f;
			for (int k = 0; k < K; k++)
			{
				z_values[L][j] += activations[L - 1][k] * weights[L][j][k];
			}
			z_values[L][j] += biases[L][j];
			activations[L][j] = sigmoid(z_values[L][j]);
		}
	}
}

void Perceptron::propagateBackwards()
{
	int numLayers = networkStructure.size();

	for (int L = numLayers - 1; L > 0; --L)
	{
		// output layer
		if (L == numLayers - 1)
		{
			int J = networkStructure[L];
			for (int j = 0; j < J; j++)
			{
				dCdz[L][j] = sigmoidDerivative(z_values[L][j]) * 2.0f * (activations[L][j] - expectedValues[j]);
			}
			continue;
		}

		// all other layers
		int K = networkStructure[L - 1];
		int J = networkStructure[L];
		int N = networkStructure[L + 1];

		for (int j = 0; j < J; j++)
		{
			dCdz[L][j] = 0.0f;
			for (int n = 0; n < N; n++)
			{
				dCdz[L][j] += weights[L][n][j] * dCdz[L + 1][n];
			}
			dCdz[L][j] *= sigmoidDerivative(z_values[L][j]);
		}
	}

	for (int L = 1; L < numLayers; L++)
	{
		// weights
		int K = networkStructure[L - 1];
		int J = networkStructure[L];

		for (int j = 0; j < J; j++)
		{
			for (int k = 0; k < K; k++)
			{
				dCdw[L][j][k] = activations[L - 1][k] * dCdz[L][j];
			}
		}
	}
}

void Perceptron::updateWeightsAndBiases()
{
	int numLayers = networkStructure.size();

	for (int L = 1; L < numLayers; L++)
	{
		int K = networkStructure[L - 1];
		int J = networkStructure[L];

		for (int j = 0; j < J; j++)
		{
			biases[L][j] -= dCdz[L][j] * learningRate;
			for (int k = 0; k < K; k++)
			{
				weights[L][j][k] -= dCdw[L][j][k] * learningRate;
			}
		}
	}
}

void Perceptron::trackTotalCost()
{
	currentCost = 0.0f;
	int numLayers = networkStructure.size();
	for (int i = 0; i < 10; i++)
	{
		float diff = activations[numLayers - 1][i] - expectedValues[i];
		currentCost += diff * diff;
	}
}

