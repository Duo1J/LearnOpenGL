#pragma once

#include <vector>

#include "Vertex.h"
#include "Texture.h"
#include "Shader.h"

/**
* 网格
*/
class Mesh
{
public:
	/**
	* 顶点
	*/
	std::vector<Vertex> vertices;

	/**
	* 索引
	*/
	std::vector<unsigned int> indices;

	/**
	* 纹理
	*/
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures);

	/**
	* 绘制
	*/
	void Draw(const Shader& shader);

private:
	/**
	* 缓冲
	*/
	unsigned int VAO, VBO, EBO;

	/**
	* 创建缓冲
	*/
	void CreateBuffer();
};

