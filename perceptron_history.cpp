
#include "perceptron.h"

void Perceptron::trackTotalCost()
{
	currentCost = 0.0f;
	int numLayers = networkStructure.size();
	for (int i = 0; i < 10; i++)
	{
		float diff = activations[numLayers - 1][i] - expectedValues[i];
		currentCost += diff * diff;
	}

	int max = 5000;
	trackCostHistory(max);
	trackAccuracyHistory(max);
}

void Perceptron::trackCostHistory(int max)
{
	static int shortRunCounter = 0;
	static float shortRunCostAccumulator = 0.0f;
	shortRunCounter++;
	shortRunCostAccumulator += currentCost;

	if (shortRunCounter >= max)
	{
		float averageCost = shortRunCostAccumulator / (float)max;
		longRunCostHistory.push_back(averageCost);
		shortRunCounter = 0;
		shortRunCostAccumulator = 0.0f;
	}
}

void Perceptron::trackAccuracyHistory(int max)
{
	static int totalPredictionsCounter = 0;
	static int correctPredictionCounter = 0;
	if (networkChoice == getCurrExpectedValue())
		correctPredictionCounter++;
	totalPredictionsCounter++;
	if (totalPredictionsCounter >= max)
	{
		accuracyHistory.push_back((float)correctPredictionCounter / (float)max);
		correctPredictionCounter = 0;
		totalPredictionsCounter = 0;
	}
}

