#pragma once
#include <GLFW/glfw3.h>
#include "global_settings.h"
#include <vector>

// Struct for representing an indexed triangle mesh
struct Mesh {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<uint32_t> indices;
};

// Struct for representing a vertex array object (VAO) created from a
// mesh. Used for rendering.
struct MeshVAO {
	GLuint vao;
	GLuint vertexVBO;
	GLuint normalVBO;
	GLuint indexVBO;
	int numVertices;
	int numIndices;
};

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

	Mesh mesh;
	MeshVAO meshVAO;

	double elapsed_time;
};
