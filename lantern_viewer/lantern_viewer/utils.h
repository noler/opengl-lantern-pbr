#pragma once 
#include <GL/glew.h>
#include <string>
#include <vector>

GLuint loadShaderProgram(const std::string &vertexShaderFilename,
	const std::string &fragmentShaderFilename);

GLuint load2DTexture(const std::string &filename);

GLuint loadCubemap(const std::string &dirname);

GLuint loadCubemapMipmap(const std::string &dirname);