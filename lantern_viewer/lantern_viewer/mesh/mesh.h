#pragma once
#include <GL/glew.h>
// #include <glm/mat4x4.hpp> // <-- will be used for projection 

#include "../data_types/context.h"

#include "../utils.h"

// The attribute locations we will use in the vertex shader
enum AttributeLocation {
	POSITION = 0,
	NORMAL = 1
};

void loadMesh(const std::string &filename, Mesh *mesh);

void createMeshVAO(Context &ctx, const Mesh &mesh, MeshVAO *meshVAO);

void drawMesh(Context &ctx, GLuint program, const MeshVAO &meshVAO);