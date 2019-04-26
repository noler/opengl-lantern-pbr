#pragma once
#include <GLFW/glfw3.h>

struct Context
{
	int width;
	int height;
	float aspect;

	GLFWwindow* window;
	GLuint program;
	GLuint program_cube;

	GLuint triangleVAO;
	GLuint positionVBO;
	GLuint colorVBO;
	GLuint defaultVAO;

	float elapsed_time;
	float zoom_factor;
};
