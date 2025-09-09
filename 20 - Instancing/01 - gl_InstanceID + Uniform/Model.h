#pragma once

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"

/**
* 模型类
*/
class Model
{
public:
	Model(const char* path);

	/**
	* 绘制
	*/
	void Draw(Shader shader, int instanceCnt = 0);

private:
	/**
	* 网格列表
	*/
	std::vector<Mesh> meshes;

	/**
	* 文件目录
	*/
	std::string directory;

	/**
	* 已加载的纹理列表
	*/
	std::vector<Texture> textures_loaded;

	/**
	* 加载模型
	*/
	void LoadModel(std::string path);

	/**
	* 处理节点
	*/
	void ProcessNode(aiNode* node, const aiScene* scene);

	/**
	* 处理网格
	*/
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	/**
	* 加载纹理
	*/
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType aiTexType, TextureType texType);
};

