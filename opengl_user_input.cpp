
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
}

