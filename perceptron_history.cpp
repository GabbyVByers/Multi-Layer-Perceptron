
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

	trackCostHistory();
	trackAccuracyHistory();
}

void Perceptron::trackCostHistory()
{
	static int shortRunCounter = 0;
	static float shortRunCostAccumulator = 0.0f;

	shortRunCounter++;
	shortRunCostAccumulator += currentCost;

	int max = 5000;
	if (shortRunCounter >= max)
	{
		float averageCost = shortRunCostAccumulator / (float)max;
		longRunCostHistory.push_back(averageCost);
		shortRunCounter = 0;
		shortRunCostAccumulator = 0.0f;
	}

}

void Perceptron::trackAccuracyHistory()
{

}

