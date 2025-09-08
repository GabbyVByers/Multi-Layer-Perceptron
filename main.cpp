
#include "perceptron.h"
#include "opengl.h"

int main()
{
	Perceptron perceptron;
	OpenGL OpenGL(1920, 1080, "Multi-Layer Perceptron", &perceptron);

	OpenGL.disableVSYNC();
	while (OpenGL.isAlive())
	{
		OpenGL.processInput();
		OpenGL.clearScreen();
		OpenGL.renderLines();
		OpenGL.renderCircles();
		OpenGL.renderGUI();
		OpenGL.swapBuffers();
	}

	return 0;
}

