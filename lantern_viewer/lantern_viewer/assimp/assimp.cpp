// Based on http://wiki.ogre3d.org/How+to+Load+Object+with+Assimp
#include "assimp.h"

ModelManager::ModelManager()
{
	meshData = std::vector<Mesh>();
}

ModelManager::~ModelManager()
{
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

Mesh ModelManager::assimpGetMeshData(const aiMesh* mesh)
{
	Mesh newMesh;

	aiFace* face;

	for (GLuint v = 0; v < mesh->mNumVertices; v++)
	{
		newMesh.vertices.push_back(
			glm::vec3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z)
		);

		newMesh.normals.push_back(
			glm::vec3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z)
		);


		if (mesh->HasTextureCoords(0))
		{
			newMesh.textureCoordinate.push_back(glm::vec2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y));
		}
		else
		{
			newMesh.textureCoordinate.push_back(glm::vec2(0.0f, 0.0f));
		}
	}

	for (GLuint f = 0; f < mesh->mNumFaces; f++)
	{
		face = &mesh->mFaces[f];
		newMesh.indices.push_back(
			face->mIndices[0]);
		newMesh.indices.push_back(
			face->mIndices[1]);
		newMesh.indices.push_back(
			face->mIndices[2]);
	}

	return newMesh;
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

	meshData.push_back(assimpGetMeshData(modelScene->mMeshes[0]));
	meshData.push_back(assimpGetMeshData(modelScene->mMeshes[1]));

	return true;
}

std::vector<Mesh> ModelManager::getMesh()
{
	return meshData;
}
