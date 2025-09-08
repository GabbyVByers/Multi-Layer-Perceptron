
#include "perceptron.h"
#include "sigmoid.h"

void Perceptron::propagateForwards()
{

}

void Perceptron::propagateBackwards()
{
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

		// weights
		for (int j = 0; j < J; j++)
		{
			for (int k = 0; k < K; k++)
			{
				dCdw[L][j][k] = activations[L - 1][k] * dCdz[L][j];
			}
		}
	}
}

