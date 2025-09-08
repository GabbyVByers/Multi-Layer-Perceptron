
#include "opengl.h"
#include "sigmoid.h"

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

	if (lineVertices.size() == 0) return;
	glBufferData(GL_ARRAY_BUFFER, sizeof(LineVertex) * lineVertices.size(), &lineVertices[0], GL_STATIC_DRAW);
	glDrawArrays(GL_LINES, 0, (GLsizei)lineVertices.size());
}

