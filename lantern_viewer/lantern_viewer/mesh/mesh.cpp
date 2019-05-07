#include "mesh.h"
#include "../utils.h"
#include <glm/gtc/type_ptr.hpp>
#include "../controls/trackball.h"

// The attribute locations we will use in the vertex shader
enum AttributeLocation
{
	POSITION = 0,
	NORMAL = 1,
	TEXTURE = 2
};

void createMeshVAO(Context& ctx, const Mesh& mesh, MeshVAO* meshVAO)
{
	// Generates and populates a VBO for the vertices
	glGenBuffers(1, &(meshVAO->vertexVBO));
	glBindBuffer(GL_ARRAY_BUFFER, meshVAO->vertexVBO);
	auto verticesNBytes = mesh.vertices.size() * sizeof(mesh.vertices[0]);
	glBufferData(GL_ARRAY_BUFFER, verticesNBytes, mesh.vertices.data(), GL_STATIC_DRAW);

	// Generates and populates a VBO for the vertex normals
	glGenBuffers(1, &(meshVAO->normalVBO));
	glBindBuffer(GL_ARRAY_BUFFER, meshVAO->normalVBO);
	auto normalsNBytes = mesh.normals.size() * sizeof(mesh.normals[0]);
	glBufferData(GL_ARRAY_BUFFER, normalsNBytes, mesh.normals.data(), GL_STATIC_DRAW);

	// Generates and populates a VBO for the element indices
	glGenBuffers(1, &(meshVAO->indexVBO));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshVAO->indexVBO);
	auto indicesNBytes = mesh.indices.size() * sizeof(mesh.indices[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesNBytes, mesh.indices.data(), GL_STATIC_DRAW);

	// Generates and populates a VBO for the texture coordinates
	glGenBuffers(1, &(meshVAO->textureVBO));
	glBindBuffer(GL_ARRAY_BUFFER, meshVAO->textureVBO);
	auto textureNBytes = mesh.textureCoordinate.size() * sizeof(mesh.textureCoordinate[0]);
	glBufferData(GL_ARRAY_BUFFER, textureNBytes, mesh.textureCoordinate.data(), GL_STATIC_DRAW);

	// Creates a vertex array object (VAO) for drawing the mesh
	glGenVertexArrays(1, &(meshVAO->vao));
	glBindVertexArray(meshVAO->vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, meshVAO->vertexVBO);
	glEnableVertexAttribArray(POSITION);
	glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	
	glBindBuffer(GL_ARRAY_BUFFER, meshVAO->normalVBO);
	glEnableVertexAttribArray(NORMAL);
	glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshVAO->indexVBO);

	glBindBuffer(GL_ARRAY_BUFFER, meshVAO->textureVBO);
	glEnableVertexAttribArray(TEXTURE);
	glVertexAttribPointer(TEXTURE, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	
	
	glBindVertexArray(ctx.defaultVAO); // unbinds the VAO

	// Additional information required by draw calls
	meshVAO->numVertices = mesh.vertices.size();
	meshVAO->numIndices = mesh.indices.size();
}

void drawMeshes(Context& ctx)
{
	ctx.lantern_obj.model = glm::translate(trackballGetRotationMatrix(ctx.trackball), glm::vec3(0.0f, -30.0f, 0.0f));;

	glDepthMask(GL_FALSE);
	drawCubeSkybox(ctx);
	glDepthMask(GL_TRUE);

	drawMesh(ctx, ctx.shader_lantern_base, ctx.lantern_obj.mesh_lantern_baseVAO, ctx.lantern_obj.model);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	drawMesh(ctx, ctx.shader_lantern_glass, ctx.lantern_obj.mesh_lantern_glassVAO, ctx.lantern_obj.model);
}

void drawMesh(Context& ctx, GLuint program, const MeshVAO& meshVAO, glm::mat4 model)
{
	glUseProgram(program);

	glActiveTexture(ctx.lantern_obj.texture.albedo);
	glBindTexture(GL_TEXTURE_2D, ctx.lantern_obj.texture.albedo);

	glm::mat4 mvp = ctx.camera.projection * ctx.camera.view * model;
	glUniformMatrix4fv(glGetUniformLocation(program, "u_mvp"),
	                   1, GL_FALSE, glm::value_ptr(mvp));

	glBindVertexArray(meshVAO.vao);
	glDrawElements(GL_TRIANGLES, meshVAO.numIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(ctx.defaultVAO);

	glUseProgram(0);
}

void updateCamera(Context& ctx)
{
	ctx.camera.projection = glm::perspective(
		glm::radians(ctx.camera.camera_projection.zoomFactor*90.0f),
		ctx.aspect,
		ctx.camera.camera_projection.zNear,
		ctx.camera.camera_projection.zFar
	);
}

void createCube(Context& ctx)
{
	// MODIFY THIS PART: Define the six faces (front, back, left,
	// right, top, and bottom) of the cube. Each face should be
	// constructed from two triangles, and each triangle should be
	// constructed from three vertices. That is, you should define 36
	// vertices that together make up 12 triangles. One triangle is
	// given; you have to define the rest!
	const GLfloat vertices[] = {
		-0.5f, 0.5f, 0.5f, // 0
		0.5, 0.5, 0.5f, // 1
		0.5, -0.5, 0.5f, // 2
		-0.5, -0.5, 0.5, // 3

		-0.5, 0.5, -0.5, // 4
		0.5, 0.5, -0.5, // 5
		0.5, -0.5, -0.5, // 6
		-0.5, -0.5, -0.5 // 7
	};

	const GLuint indices[] = {
		// front face
		1, 2, 3,
		0, 1, 3,
		// back face
		4, 5, 6,
		6, 4, 7,
		// left face
		0, 3, 7,
		0, 7, 4,
		// right face
		2, 1, 6,
		6, 1, 5,
		// top face
		0, 5, 1,
		5, 0, 4,
		// bottom face
		3, 6, 2,
		3, 7, 6
	};

	glGenBuffers(1, &ctx.skybox_obj.skyboxIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx.skybox_obj.skyboxIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Generates and populates a vertex buffer object (VBO) for the
	// vertices (DO NOT CHANGE THIS)
	glGenBuffers(1, &ctx.skybox_obj.skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, ctx.skybox_obj.skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Creates a vertex array object (VAO) for drawing the cube
	// (DO NOT CHANGE THIS)
	glGenVertexArrays(1, &ctx.skybox_obj.skyboxVAO);
	glBindVertexArray(ctx.skybox_obj.skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, ctx.skybox_obj.skyboxVBO);
	glEnableVertexAttribArray(POSITION);
	glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindVertexArray(ctx.defaultVAO); // unbinds the VAO
}


void drawCubeSkybox(Context& ctx)
{
	glUseProgram(ctx.shader_skybox);
	glm::mat4 model = trackballGetRotationMatrix(ctx.trackball);

	model = glm::scale(
		model,
		glm::vec3(
			300.5,
			300.5,
			300.5)
	);

	// model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 view = glm::lookAt(
		glm::vec3(3.2, 0, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(0.0, 1.0, 0)
	);

	glm::mat4 projection = glm::ortho(-1.7f, 1.7f, -1.7f, 1.7f, 0.0f, 10.0f);

	glActiveTexture(ctx.skybox_obj.skybox_cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ctx.skybox_obj.skybox_cubemap);

	glm::mat4 mvp = ctx.camera.projection * ctx.camera.view * model;

	glUniformMatrix4fv(glGetUniformLocation(ctx.shader_skybox, "u_mvp"),
		1, GL_FALSE, glm::value_ptr(mvp));

	glBindVertexArray(ctx.skybox_obj.skyboxVAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx.skybox_obj.skyboxIBO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(ctx.defaultVAO);

	glUseProgram(0);
}