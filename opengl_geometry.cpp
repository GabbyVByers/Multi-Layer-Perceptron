
#include "opengl.h"
#include "random.h"

struct DrawingParameters
{
	float radius = 0.018f;
	float margin = 0.0012f;
	float stride = 0.4f;
	float spacing = 0.045f;
	float output_spacing = 0.075f;
};

void OpenGL::constructNetworkGeometry()
{
	DrawingParameters dp;

	for (int L = 0; L < perceptron->networkStructure.size(); L++)
	{
		std::vector<Vec2f> layer;
		layer.resize(perceptron->networkStructure[L]);
		networkGeometry.push_back(layer);
	}

	for (int j = 0; j < 784; j++)
	{
		Vec2f position;
		position.x = (-27.0f * dp.spacing) + ((j % 28) * dp.spacing);
		position.y = (-13.5f * dp.spacing) + ((j / 28) * dp.spacing);
		networkGeometry[0][j] = position;
	}

	int numLayers = perceptron->networkStructure.size();
	for (int L = 1; L < numLayers; L++)
	{
		float spacing = (L == numLayers - 1) ? dp.output_spacing : dp.spacing;
		int J = perceptron->networkStructure[L];
		for (int j = 0; j < J; j++)
		{
			Vec2f position;
			position.x = dp.stride * (float)L;
			position.y = (spacing * j) - (spacing * (J - 1) * 0.5f);
			networkGeometry[L][j] = position;
		}
	}

	for (int L = 0; L < numLayers; L++)
	{
		auto& layer = networkGeometry[L];
		int J = perceptron->networkStructure[L];
		for (int j = 0; j < J; j++)
		{
			auto& neuron_pos = layer[j];
			float activation = perceptron->activations[L][j];
			circleVertices.push_back({ neuron_pos.x, neuron_pos.y, 1.0f, dp.radius });
			circleVertices.push_back({ neuron_pos.x, neuron_pos.y, activation, dp.radius - dp.margin });
		}
	}

	for (int L = 1; L < numLayers; L++)
	{
		auto& previous_layer_geometry = networkGeometry[L - 1];
		auto& layer_geometry = networkGeometry[L];

		int K = perceptron->networkStructure[L - 1];
		int J = perceptron->networkStructure[L];

		for (int j = 0; j < J; j++)
		{
			Vec2f& neuron_pos = layer_geometry[j];
			for (int k = 0; k < K; k++)
			{
				Vec2f& prevous_neuron_pos = previous_layer_geometry[k];
				float weight = perceptron->weights[L][j][k];
				float trans = (L == 1) ? 0.1f : 0.4f;
				lineVertices.push_back({ neuron_pos.x, neuron_pos.y, weight, trans });
				lineVertices.push_back({ prevous_neuron_pos.x, prevous_neuron_pos.y, weight, trans });
			}
		}
	}
}

