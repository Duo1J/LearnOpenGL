#pragma once

#include <glm.hpp>

/**
* 顶点
*/
struct Vertex
{
	/**
	* 位置
	*/
	glm::vec3 position;

	/**
	* 法向量
	*/
	glm::vec3 normal;

	/**
	* UV
	*/
	glm::vec2 texCoord;

	/**
	* 切线
	*/
	glm::vec3 tangent;

	/**
	* 副切线
	*/
	glm::vec3 bitangent;
};