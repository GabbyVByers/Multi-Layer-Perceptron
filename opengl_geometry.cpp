
#include "opengl.h"
#include "random.h"

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
		position.y = ( 13.5f * dp.spacing) - ((j / 28) * dp.spacing);
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
}

void OpenGL::renderNetwork()
{
	profiler.start();
	renderLines();
	renderCircles();
	renderGUI();
	profiler.stop();
	renderingTime = profiler.time();
}

