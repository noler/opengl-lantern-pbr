#include "mesh.h"
#include "../utils.h"
#include <glm/gtc/type_ptr.hpp>
#include "../data_types/trackball.h"

// The attribute locations we will use in the vertex shader
enum AttributeLocation
{
	POSITION = 0,
	NORMAL = 1
};

void loadMesh(const std::string& filename, Mesh* mesh)
{
	OBJMesh obj_mesh;
	objMeshLoad(obj_mesh, filename);
	mesh->vertices = obj_mesh.vertices;
	mesh->normals = obj_mesh.normals;
	mesh->indices = obj_mesh.indices;
}

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

// Get trackball orientation in matrix form
glm::mat4 trackballGetRotationMatrix(Trackball &trackball)
{
	return glm::mat4_cast(trackball.qCurrent);
}

void drawMesh(Context& ctx, GLuint program, const MeshVAO& meshVAO)
{
	glUseProgram(program);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -20.0f, 0.0f));
	model = model * trackballGetRotationMatrix(ctx.trackball);
	glm::mat4 view = glm::lookAt(
		glm::vec3(0, 0.0, 100.0),
		glm::vec3(0, 0, 0),
		glm::vec3(0.0, 1.0, 0.0)
	);


	glm::mat4 projection = glm::perspective(
		glm::radians(90.0f),
		ctx.aspect,
		0.1f,
		200.0f
	);

	glm::mat4 mvp = projection * view * model;
	glUniformMatrix4fv(glGetUniformLocation(program, "u_mvp"),
	                   1, GL_FALSE, glm::value_ptr(mvp));

	glBindVertexArray(meshVAO.vao);
	glDrawElements(GL_TRIANGLES, meshVAO.numIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(ctx.defaultVAO);

	glUseProgram(0);
}
