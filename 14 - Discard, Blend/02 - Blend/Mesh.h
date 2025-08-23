#pragma once

#include <vector>

#include "Vertex.h"
#include "Texture.h"
#include "Shader.h"

/**
* ����
*/
class Mesh
{
public:
	/**
	* ����
	*/
	std::vector<Vertex> vertices;

	/**
	* ����
	*/
	std::vector<unsigned int> indices;

	/**
	* ����
	*/
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures);

	/**
	* ����
	*/
	void Draw(const Shader& shader);

private:
	/**
	* ����
	*/
	unsigned int VAO, VBO, EBO;

	/**
	* ��������
	*/
	void CreateBuffer();
};

