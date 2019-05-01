// Based on http://wiki.ogre3d.org/How+to+Load+Object+with+Assimp
#include "assimp.h"

ModelManager::ModelManager()
{
	vertexBuff.clear();
	indexBuff.clear();

	meshData = Mesh();

	std::cout << "++++++++++++ Mesh Manager Initilized +++++++++++++" << std::endl;
}

ModelManager::~ModelManager()
{
	std::cout << "------------ Mesh Manager Destroyed ------------" << std::endl;
}

bool ModelManager::loadModel(std::string filePath)
{
	modelScene = importer.ReadFile(filePath,
	                               aiProcess_MakeLeftHanded |
	                               aiProcess_FlipWindingOrder |
	                               aiProcess_FlipUVs |
	                               aiProcess_PreTransformVertices |
	                               aiProcess_CalcTangentSpace |
	                               aiProcess_GenSmoothNormals |
	                               aiProcess_Triangulate |
	                               aiProcess_FixInfacingNormals |
	                               aiProcess_FindInvalidData |
	                               aiProcess_ValidateDataStructure | 0
	);

	if (!modelScene)
	{
		MessageBoxA(NULL, importer.GetErrorString(), "Error: ", MB_ICONERROR);
		return false;
	}

	processData();
	return true;
}

bool ModelManager::assimpGetMeshData(const aiMesh* mesh)
{
	aiFace* face;

	for (GLuint v = 0; v < mesh->mNumVertices; v++)
	{
		meshData.vertices.push_back(
			glm::vec3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z)
		);

		meshData.normals.push_back(
			glm::vec3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z)
		);
	}

	for (GLuint f = 0; f < mesh->mNumFaces; f++)
	{
		face = &mesh->mFaces[f];
		meshData.indices.push_back(
			face->mIndices[0]);
		meshData.indices.push_back(
			face->mIndices[1]);
		meshData.indices.push_back(
			face->mIndices[2]);
	}

	return true;
}

bool ModelManager::processData()
{
	bool repeat = true;
	nodeBuff.push_back(modelScene->mRootNode);

	while (repeat)
	{
		for (GLuint i = 0; i < nodeBuff.size(); i++)
		{
			modelNode = nodeBuff.at(i);
			if (modelNode->mNumChildren > 0)
			{
				for (GLuint c = 0; c < modelNode->mNumChildren; c++)
				{
					nodeBuff.push_back(modelNode->mChildren[c]);
				}
			}
			else
			{
				repeat = false;
			}
		}
	}

	for (GLuint i = 0; i < nodeBuff.size(); i++)
	{
		modelNode = nodeBuff.at(i);

		if (modelNode->mNumMeshes > 0)
		{
			for (GLuint j = 0; j < modelNode->mNumMeshes; j++)
			{
				assimpGetMeshData(modelScene->mMeshes[j]);
			}
		}
	}
	return true;
}

std::vector<float>* ModelManager::getVertexData()
{
	return &vertexBuff;
}

std::vector<GLuint>* ModelManager::getIndexData()
{
	return &indexBuff;
}

Mesh ModelManager::getMesh()
{
	return meshData;
}