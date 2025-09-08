#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "perceptron.h"
#include "profiler.h"
#include "vec2f.h"

struct DrawingParameters
{
	float radius = 0.020f;
	float margin = 0.0012f;
	float stride = 0.4f;
	float spacing = 0.045f;
	float output_spacing = 0.075f;
};

struct CircleVertex
{
	float x, y;
	float color;
	float radius;
};

struct LineVertex
{
	float x, y;
	float color;
	float trans;
};

class OpenGL
{
public:
	
	GLFWwindow* window = nullptr;
	Profiler profiler;
	double renderingTime = 0.0;

	Perceptron* perceptron = nullptr;
	bool drawInputLayerWeights = true;
	std::vector<std::vector<Vec2f>> networkGeometry;
	std::vector<CircleVertex> circleVertices;
	std::vector<LineVertex> lineVertices;

	unsigned int circleShaderProgram;
	unsigned int circleVAO;
	unsigned int circleVBO;

	unsigned int lineShaderProgram;
	unsigned int lineVAO;
	unsigned int lineVBO;

	// utilities
	OpenGL(int initWidth, int initHeight, std::string title, Perceptron* ptr);
	~OpenGL();
	bool isAlive();
	void disableVSYNC();
	void enableVSYNC();
	void clearScreen();
	void swapBuffers();
	std::string loadSourceFile(std::string filePath);

	// rendering
	void constructNetworkGeometry();
	void renderNetwork();

	// circles
	void initCircleRendering();
	void renderCircles();

	// lines
	void initLineRendering();
	void renderLines();

	// gui
	void initImGui() const;
	void renderGUI();

	// user_input
	void processInput();
};

