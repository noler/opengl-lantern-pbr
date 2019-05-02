#pragma once 
#include <GL/glew.h>
#include <string>
#include <vector>

GLuint loadShaderProgram(const std::string &vertexShaderFilename,
	const std::string &fragmentShaderFilename);
