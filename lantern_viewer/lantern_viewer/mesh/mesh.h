#pragma once
#include <GL/glew.h>

#include "../data_types/context.h"

void loadMesh(const std::string &filename, Mesh *mesh);

void createMeshVAO(Context &ctx, const Mesh &mesh, MeshVAO *meshVAO);

void drawMesh(Context &ctx, GLuint program, const MeshVAO &meshVAO);
