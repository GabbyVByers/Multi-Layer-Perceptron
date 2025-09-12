
#include "opengl.h"

void OpenGL::initLineRendering()
{
	glGenVertexArrays(1, &lineVAO);
	glGenBuffers(1, &lineVBO);

	glBindVertexArray(lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	std::string vert = loadSourceFile("glsl_line.vert");
	std::string frag = loadSourceFile("glsl_line.frag");
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

	lineShaderProgram = glCreateProgram();
	glAttachShader(lineShaderProgram, vertexShader);
	glAttachShader(lineShaderProgram, fragmentShader);
	glLinkProgram(lineShaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGL::renderLines()
{
	int width, height; glfwGetFramebufferSize(window, &width, &height);
	glBindVertexArray(lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glUseProgram(lineShaderProgram);
	glUniform2f(glGetUniformLocation(lineShaderProgram, "uScreenDimensions"), (float)width, (float)height);

	lineVertices.clear();
	int numLayers = perceptron->networkStructure.size();
	int startLayer = (drawInputLayerWeights) ? 1 : 2;
	for (int L = startLayer; L < numLayers; L++)
	{
		std::vector<Vec2f>& previous_layer_geometry = networkGeometry[L - 1];
		std::vector<Vec2f>& layer_geometry = networkGeometry[L];

		int K = perceptron->networkStructure[L - 1];
		int J = perceptron->networkStructure[L];

		for (int j = 0; j < J; j++)
		{
			Vec2f& neuron_pos = layer_geometry[j];
			for (int k = 0; k < K; k++)
			{
				Vec2f& prevous_neuron_pos = previous_layer_geometry[k];
				float weight = perceptron->weights[L][j][k];
				float trans = (L == 1) ? 0.1f : 0.35f;
				lineVertices.push_back({ neuron_pos.x, neuron_pos.y, weight, trans });
				lineVertices.push_back({ prevous_neuron_pos.x, prevous_neuron_pos.y, weight, trans });
			}
		}
	}

	std::vector<Vec2f> arrowOffsets =
	{
		{ 1.0f,  0.0f },
		{ 2.0f,  1.0f },
		{ 1.0f,  0.0f },
		{ 2.0f, -1.0f },
		{ 2.0f, -1.0f },
		{ 2.0f, -0.3f },
		{ 2.0f,  1.0f },
		{ 2.0f,  0.3f },
		{ 2.0f, -0.3f },
		{ 3.0f, -0.3f },
		{ 2.0f,  0.3f },
		{ 3.0f,  0.3f },
		{ 3.0f, -0.3f },
		{ 3.0f,  0.3f }
	};

	Vec2f& choicePos = networkGeometry[numLayers - 1][perceptron->networkChoice];
	for (int i = 0; i < arrowOffsets.size(); i++)
	{
		lineVertices.push_back(
			{
				choicePos.x + (arrowOffsets[i].x * dp.spacing),
				choicePos.y + (arrowOffsets[i].y * dp.spacing),
				20.0f, 1.0f
			}
		);
	}

	if (lineVertices.size() == 0) return;
	glBufferData(GL_ARRAY_BUFFER, sizeof(LineVertex) * lineVertices.size(), &lineVertices[0], GL_STATIC_DRAW);
	glDrawArrays(GL_LINES, 0, (GLsizei)lineVertices.size());
}

