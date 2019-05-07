#pragma once
#include <GL/glew.h>

#include "../data_types/context.h"

void loadMesh(const std::string &filename, Mesh *mesh);

void createMeshVAO(Context &ctx, const Mesh &mesh, MeshVAO *meshVAO);

void drawMesh(Context &ctx, GLuint program, const MeshVAO &meshVAO, glm::mat4 model);

void drawMeshes(Context& ctx);

void updateCamera(Context& ctx);

void createCube(Context& ctx);

void drawCubeSkybox(Context& ctx);