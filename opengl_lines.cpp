
#include "opengl.h"
#include "sigmoid.h"

struct LineVertex
{
	float x, y;
	float r, g, b;
};

void OpenGL::initLineRendering()
{
	glGenVertexArrays(1, &lineVAO);
	glGenBuffers(1, &lineVBO);

	glBindVertexArray(lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

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
}

void OpenGL::renderLines()
{
	glBindVertexArray(lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glUseProgram(lineShaderProgram);
	std::vector<LineVertex> lineVertices;



	if (lineVertices.size() == 0) return;
	glBufferData(GL_ARRAY_BUFFER, sizeof(LineVertex) * lineVertices.size(), &lineVertices[0], GL_STATIC_DRAW);
	int width, height; glfwGetFramebufferSize(window, &width, &height);
	glUniform2f(glGetUniformLocation(lineShaderProgram, "uScreenDimensions"), (float)width, (float)height);
	glDrawArrays(GL_LINES, 0, (GLsizei)lineVertices.size());
}

