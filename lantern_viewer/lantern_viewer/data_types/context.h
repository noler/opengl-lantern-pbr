#pragma once
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

#include "global_settings.h"
#include "material_settings.h"

// Struct for trackball
struct Trackball
{
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
	{
	}
};

struct Texture
{
	GLuint albedo;
	GLuint ambient_occlusion;
	GLuint metallic;
	GLuint normal;
	GLuint opacity;
	GLuint roughness;
};

// Struct for representing an indexed triangle mesh
struct Mesh
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangent;
	std::vector<glm::vec3> bitangent;
	std::vector<glm::vec2> textureCoordinate;
	std::vector<uint32_t> indices;
};


// Struct for representing a vertex array object (VAO) created from a
// mesh. Used for rendering.
struct MeshVAO
{
	GLuint vao;
	GLuint vertexVBO;
	GLuint normalVBO;
	GLuint tangentVBO;
	GLuint bitangentVBO;
	GLuint indexVBO;
	GLuint textureVBO;
	int numVertices;
	int numIndices;
};

struct LanternOBJ
{
	Mesh mesh_lantern_base;
	MeshVAO mesh_lantern_baseVAO;

	Mesh mesh_lantern_glass;
	MeshVAO mesh_lantern_glassVAO;

	Texture texture;

	glm::mat4 model = glm::mat4(1.0f);
};

struct CameraProjection
{
	float zNear;
	float zFar;
	float zoomFactor;
};

struct Camera
{
	glm::vec3 position;
	glm::mat4 view;
	glm::mat4 projection;
	CameraProjection camera_projection;
};

struct SkyboxOBJ
{
	GLuint skyboxIBO;
	GLuint skyboxVBO;
	GLuint skyboxVAO;

	GLuint skybox_cubemap;
	GLuint skybox_cubemap_mipmap;
};

struct SphereOBJ
{
	Mesh mesh_sphere;
	MeshVAO mesh_sphere_VAO;
	Texture texture;
};

struct Light
{
	float strength = 1.0;
	glm::vec3 position;
	glm::vec3 color;
};

struct Context
{
	GLFWwindow* window;
	GLuint shader_lantern_base;
	GLuint shader_lantern_glass;
	GLuint shader_skybox;

	GlobalSettings global_settings;

	MaterialSettings material_settings;

	float aspect;

	GLuint triangleVAO;
	GLuint positionVBO;
	GLuint colorVBO;
	GLuint defaultVAO;

	Camera camera;

	LanternOBJ lantern_obj;
	SkyboxOBJ skybox_obj;
	SkyboxOBJ skybox_reverse_obj;

	SphereOBJ sphere_obj;

	Trackball trackball;

	Light lights;

	bool lantern_on = 1;

	double elapsed_time;
};
