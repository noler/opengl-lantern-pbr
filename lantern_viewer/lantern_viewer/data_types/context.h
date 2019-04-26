#pragma once
#include <GLFW/glfw3.h>
#include "global_settings.h"

struct Context
{
	GLFWwindow* window;
	GLuint shader_program;

	GlobalSettings global_settings;

	float aspect;

	GLuint triangleVAO;
	GLuint positionVBO;
	GLuint colorVBO;
	GLuint defaultVAO;

	double elapsed_time;
};
