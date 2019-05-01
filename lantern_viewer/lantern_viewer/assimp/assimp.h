#pragma once
// Based from http://wiki.ogre3d.org/How+to+Load+Object+with+Assimp

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>
#include <assimp/cimport.h>

#include <Windows.h>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include "../mesh/mesh.h"

class ModelManager
{
public:
	ModelManager();
	~ModelManager(void);

	bool loadModel(std::string filePath);
	bool processData();
	Mesh getMesh();
	std::vector<float>* getVertexData();
	std::vector<GLuint>* getIndexData();

private:
	bool assimpGetMeshData(const aiMesh* mesh);
	Assimp::Importer importer;
	const aiScene* modelScene;
	const aiNode* modelNode;
	const aiMesh* modelMesh;
	const aiFace* modelFace;
	std::vector<float> vertexBuff;
	std::vector<GLuint> indexBuff;
	Mesh meshData;

	std::vector<const aiNode*> nodeBuff;
	GLuint numNodeBuff;
};
