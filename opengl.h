#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
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
	float margin = 0.002f;
	float stride = 0.2f;
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
	Profiler FPSTracker;
	double renderingTime = 0.0;
	double mainLoopTime = 0.0;
	DrawingParameters dp;
	Perceptron* perceptron = nullptr;
	bool drawInputLayerWeights = true;

	std::vector<std::vector<Vec2f>> networkGeometry;
	std::vector<CircleVertex> circleVertices;
	std::vector<LineVertex> lineVertices;
	Vec2f globalOffset;

	unsigned int circleShaderProgram;
	unsigned int circleVAO;
	unsigned int circleVBO;

	unsigned int lineShaderProgram;
	unsigned int lineVAO;
	unsigned int lineVBO;

	// opengl_constructor.cpp
	OpenGL(int initWidth, int initHeight, std::string title, Perceptron* ptr);
	~OpenGL();
	bool isAlive();
	void disableVSYNC();
	void enableVSYNC();
	void clearScreen();
	void swapBuffers();
	std::string loadSourceFile(std::string filePath);

	// opengl_geometry.cpp
	void constructNetworkGeometry();
	void renderNetwork();

	// opengl_circles.cpp
	void initCircleRendering();
	void renderCircles();

	// opengl_lines.cpp
	void initLineRendering();
	void renderLines();

	// opengl_gui.cpp
	void initImGui() const;
	void renderGUI();

	// opengl_user_input.cpp
	void processInput();
};

