#include "mesh.h"
#include "../utils.h"
#include <glm/gtc/type_ptr.hpp>
#include "../controls/trackball.h"

// The attribute locations we will use in the vertex shader
enum AttributeLocation
{
	POSITION = 0,
	NORMAL = 1
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
	glBindVertexArray(ctx.defaultVAO); // unbinds the VAO

	// Additional information required by draw calls
	meshVAO->numVertices = mesh.vertices.size();
	meshVAO->numIndices = mesh.indices.size();
}

void drawMeshes(Context& ctx)
{
	ctx.lantern_obj.model = glm::translate(trackballGetRotationMatrix(ctx.trackball), glm::vec3(0.0f, -30.0f, 0.0f));;

	drawMesh(ctx, ctx.shader_lantern_base, ctx.lantern_obj.mesh_lantern_baseVAO, ctx.lantern_obj.model);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	drawMesh(ctx, ctx.shader_lantern_glass, ctx.lantern_obj.mesh_lantern_glassVAO, ctx.lantern_obj.model);

}

void drawMesh(Context& ctx, GLuint program, const MeshVAO& meshVAO, glm::mat4 model)
{
	glUseProgram(program);
	

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