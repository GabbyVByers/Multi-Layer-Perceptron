
#include "opengl.h"
#include "random.h"

void OpenGL::initCircleRendering()
{
	glGenVertexArrays(1, &circleVAO);
	glGenBuffers(1, &circleVBO);

	glBindVertexArray(circleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, circleVBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	std::string vert = loadSourceFile("glsl_circle.vert");
	std::string frag = loadSourceFile("glsl_circle.frag");
	const char* vertShaderSource = vert.c_str();
	const char* fragShaderSource = frag.c_str();

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertShaderSource, nullptr);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderSource, nullptr);
	glCompileShader(fragmentShader);

	circleShaderProgram = glCreateProgram();
	glAttachShader(circleShaderProgram, vertexShader);
	glAttachShader(circleShaderProgram, fragmentShader);
	glLinkProgram(circleShaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGL::renderCircles()
{
	int width, height; glfwGetFramebufferSize(window, &width, &height);
	glBindVertexArray(circleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
	glUseProgram(circleShaderProgram);
	glUniform2f(glGetUniformLocation(circleShaderProgram, "uScreenDimensions"), (float)width, (float)height);
	
	DrawingParameters dp;
	circleVertices.clear();
	int numLayers = perceptron->networkStructure.size();
	for (int L = 0; L < numLayers; L++)
	{
		std::vector<Vec2f>& layer = networkGeometry[L];
		int J = perceptron->networkStructure[L];
		for (int j = 0; j < J; j++)
		{
			Vec2f& neuron_pos = layer[j];
			float activation = perceptron->activations[L][j];
			circleVertices.push_back({ neuron_pos.x, neuron_pos.y, 1.0f, dp.radius });
			circleVertices.push_back({ neuron_pos.x, neuron_pos.y, activation, dp.radius - dp.margin });
		}
	}

	for (int j = 0; j < 10; j++)
	{
		Vec2f neuron_pos = networkGeometry[numLayers - 1][j];
		neuron_pos.x += dp.stride;
		float trueValue = (j == perceptron->getCurrExpectedValue())? 1.0f : 0.0f;
		circleVertices.push_back({ neuron_pos.x, neuron_pos.y, 1.0f, dp.radius });
		circleVertices.push_back({ neuron_pos.x, neuron_pos.y, trueValue, dp.radius - dp.margin });
	}

	if (circleVertices.size() == 0) return;
	glBufferData(GL_ARRAY_BUFFER, sizeof(CircleVertex) * circleVertices.size(), &circleVertices[0], GL_STATIC_DRAW);
	glDrawArrays(GL_POINTS, 0, (GLsizei)circleVertices.size());
}

