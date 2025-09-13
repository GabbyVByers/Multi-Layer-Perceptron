
#include "perceptron.h"
#include "opengl.h"

int main()
{
	Perceptron perceptron;
	OpenGL OpenGL(1920, 1080, "Multi-Layer Perceptron", &perceptron);

	OpenGL.disableVSYNC();
	while (OpenGL.isAlive())
	{
		if (perceptron.isContinuouslyTraining)
			perceptron.continuouslyTrain();

		if (perceptron.isBeingBenchmarked)
			perceptron.benckmarkNetworkAgainstTestExamples();

		if (perceptron.isInDrawingMode)
			perceptron.propagateForwards();

		OpenGL.processInput();
		OpenGL.clearScreen();
		OpenGL.renderNetwork();
		OpenGL.swapBuffers();
	}

	return 0;
}

