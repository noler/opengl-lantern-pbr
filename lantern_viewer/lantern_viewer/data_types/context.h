#pragma once
#include <GLFW/glfw3.h>
#include "global_settings.h"
#include <glm/vec3.hpp>
#include <vector>
#include "trackball.h"

struct Trackball {
	double radius;
	glm::vec2 center;
	bool tracking;
	glm::vec3 vStart;
	glm::quat qStart;
	glm::quat qCurrent;

	Trackball() : radius(1.0),
		center(glm::vec2(0.0f, 0.0f)),
		tracking(false),
		vStart(glm::vec3(0.0f, 0.0f, 1.0f)),
		qStart(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
		qCurrent(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
	{}
};

// Struct for representing an indexed triangle mesh
struct Mesh {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoordinate;
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
	GLuint shader_lantern_base;
	GLuint shader_lantern_glass;

	GlobalSettings global_settings;

	float aspect;

	GLuint triangleVAO;
	GLuint positionVBO;
	GLuint colorVBO;
	GLuint defaultVAO;

	Mesh mesh_lantern_base;
	MeshVAO mesh_lantern_baseVAO;

	Mesh mesh_lantern_glass;
	MeshVAO mesh_lantern_glassVAO;

	Trackball trackball;

	double elapsed_time;
};
