#pragma once

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"

/**
* ģ����
*/
class Model
{
public:
	Model(const char* path);

	/**
	* ����
	*/
	void Draw(Shader shader, int instanceCnt = 0);

private:
	/**
	* �����б�
	*/
	std::vector<Mesh> meshes;

	/**
	* �ļ�Ŀ¼
	*/
	std::string directory;

	/**
	* �Ѽ��ص������б�
	*/
	std::vector<Texture> textures_loaded;

	/**
	* ����ģ��
	*/
	void LoadModel(std::string path);

	/**
	* ����ڵ�
	*/
	void ProcessNode(aiNode* node, const aiScene* scene);

	/**
	* ��������
	*/
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	/**
	* ��������
	*/
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType aiTexType, TextureType texType);
};

