
#include "opengl.h"

void OpenGL::processInput()
{
	double mouseX, mouseY; glfwGetCursorPos(window, &mouseX, &mouseY);
	int width, height; glfwGetFramebufferSize(window, &width, &height);
	float max_u = (float)width / (float)height;

	Vec2f currMousePosition =
	{
		((2.0f * (float)mouseX * max_u) / (float)width) - max_u,
		(-2.0f * ((float)mouseY / (float)height)) + 1.0f
	};
	static Vec2f prevMousePosition = { 0.0f, 0.0f };
	Vec2f relativeMouseMovement = currMousePosition - prevMousePosition;
	prevMousePosition = currMousePosition;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
		globalOffset = globalOffset + relativeMouseMovement;

	if (perceptron->isInDrawingMode)
	{
		if ((relativeMouseMovement.x != 0.0f) || (relativeMouseMovement.y != 0.0f))
		{
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				DrawingParameters dp;
				for (int j = 0; j < 784; j++)
				{
					Vec2f position = networkGeometry[0][j];
					float dx = position.x - currMousePosition.x;
					float dy = position.y - currMousePosition.y;
					float distToMouse = sqrt((dx * dx) + (dy * dy));
					float radius = 1.5f * dp.spacing;
					if (distToMouse < radius)
					{
						float frac = radius / distToMouse;
						perceptron->activations[0][j] += frac * 0.075f;
					}
				}
			}

			else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{
				DrawingParameters dp;
				for (int j = 0; j < 784; j++)
				{
					Vec2f position = networkGeometry[0][j];
					float dx = position.x - currMousePosition.x;
					float dy = position.y - currMousePosition.y;
					float distToMouse = sqrt((dx * dx) + (dy * dy));
					float radius = 3.5f * dp.spacing;
					if (distToMouse < radius)
					{
						perceptron->activations[0][j] = 0.0f;
					}
				}
			}
		}
	}
}

