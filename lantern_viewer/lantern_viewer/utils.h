#include <GL/glew.h>
#include <string>
#include <vector>
#include <glm/vec3.hpp>

// Struct for Wavefront (OBJ) triangle meshes that are indexed and has
// per-vertex normals
struct OBJMesh {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<std::uint32_t> indices;
};


GLuint loadShaderProgram(const std::string &vertexShaderFilename,
	const std::string &fragmentShaderFilename);

bool objMeshLoad(OBJMesh &mesh, const std::string &filename);
