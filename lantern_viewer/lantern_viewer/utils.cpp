#include "utils.h"
#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

std::string readShaderSource(const std::string &filename)
{
	std::ifstream file(filename);
	std::stringstream stream;
	stream << file.rdbuf();

	return stream.str();
}

void showShaderInfoLog(GLuint shader)
{
	GLint infoLogLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> infoLog(infoLogLength);
	glGetShaderInfoLog(shader, infoLogLength, &infoLogLength, &infoLog[0]);
	std::string infoLogStr(infoLog.begin(), infoLog.end());
	std::cerr << infoLogStr << std::endl;
}

void showProgramInfoLog(GLuint program)
{
	GLint infoLogLength = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> infoLog(infoLogLength);
	glGetProgramInfoLog(program, infoLogLength, &infoLogLength, &infoLog[0]);
	std::string infoLogStr(infoLog.begin(), infoLog.end());
	std::cerr << infoLogStr << std::endl;
}

GLuint loadShaderProgram(const std::string &vertexShaderFilename,
	const std::string &fragmentShaderFilename)
{
	// Load and compile vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertexShaderSource = readShaderSource(vertexShaderFilename);
	const char *vertexShaderSourcePtr = vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderSourcePtr, nullptr);

	glCompileShader(vertexShader);
	GLint compiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		std::cerr << "Vertex shader compilation failed:" << std::endl;
		showShaderInfoLog(vertexShader);
		glDeleteShader(vertexShader);
		return 0;
	}

	// Load and compile fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragmentShaderSource = readShaderSource(fragmentShaderFilename);
	const char *fragmentShaderSourcePtr = fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSourcePtr, nullptr);

	glCompileShader(fragmentShader);
	compiled = 0;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		std::cerr << "Fragment shader compilation failed:" << std::endl;
		showShaderInfoLog(fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return 0;
	}

	// Create program object
	GLuint program = glCreateProgram();

	// Attach shaders to the program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	// Link program
	glLinkProgram(program);

	// Check linking status
	GLint linked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
		std::cerr << "Linking failed:" << std::endl;
		showProgramInfoLog(program);
		glDeleteProgram(program);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return 0;
	}

	// Clean up
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	return program;
}

void computeNormals(const std::vector<glm::vec3> &vertices,
	const std::vector<std::uint32_t> &indices,
	std::vector<glm::vec3> *normals)
{
	normals->resize(vertices.size(), glm::vec3(0.0f, 0.0f, 0.0f));

	// Compute per-vertex normals by averaging the unnormalized face normals
	std::uint32_t vertexIndex0, vertexIndex1, vertexIndex2;
	glm::vec3 normal;
	int numIndices = indices.size();
	for (int i = 0; i < numIndices; i += 3) {
		vertexIndex0 = indices[i];
		vertexIndex1 = indices[i + 1];
		vertexIndex2 = indices[i + 2];
		normal = glm::cross(vertices[vertexIndex1] - vertices[vertexIndex0],
			vertices[vertexIndex2] - vertices[vertexIndex0]);
		(*normals)[vertexIndex0] += normal;
		(*normals)[vertexIndex1] += normal;
		(*normals)[vertexIndex2] += normal;
	}

	int numNormals = normals->size();
	for (int i = 0; i < numNormals; i++) {
		(*normals)[i] = glm::normalize((*normals)[i]);
	}
}

// Read an OBJMesh from an .obj file
bool objMeshLoad(OBJMesh &mesh, const std::string &filename)
{
	const std::string VERTEX_LINE("v ");
	const std::string FACE_LINE("f ");

	// Open OBJ file
	std::ifstream f(filename.c_str());
	if (!f.is_open()) {
		std::cerr << "Could not open " << filename << std::endl;
		return false;
	}

	// Extract vertices and indices
	std::string line;
	glm::vec3 vertex;
	std::uint32_t vertexIndex0, vertexIndex1, vertexIndex2;
	while (!f.eof()) {
		std::getline(f, line);
		if (line.substr(0, 2) == VERTEX_LINE) {
			std::istringstream vertexLine(line.substr(2));
			vertexLine >> vertex.x;
			vertexLine >> vertex.y;
			vertexLine >> vertex.z;
			mesh.vertices.push_back(vertex);
		}
		else if (line.substr(0, 2) == FACE_LINE) {
			std::istringstream faceLine(line.substr(2));
			faceLine >> vertexIndex0;
			faceLine >> vertexIndex1;
			faceLine >> vertexIndex2;
			mesh.indices.push_back(vertexIndex0 - 1);
			mesh.indices.push_back(vertexIndex1 - 1);
			mesh.indices.push_back(vertexIndex2 - 1);
		}
		else {
			// Ignore line
		}
	}

	// Close OBJ file
	f.close();

	// Compute normals
	computeNormals(mesh.vertices, mesh.indices, &mesh.normals);

	// Display log message
	std::cout << "Loaded OBJ file " << filename << std::endl;
	int numTriangles = mesh.indices.size() / 3;
	std::cout << "Number of triangles: " << numTriangles << std::endl;

	return true;
}