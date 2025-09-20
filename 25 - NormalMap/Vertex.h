#pragma once

#include <glm.hpp>

/**
* ����
*/
struct Vertex
{
	/**
	* λ��
	*/
	glm::vec3 position;

	/**
	* ������
	*/
	glm::vec3 normal;

	/**
	* UV
	*/
	glm::vec2 texCoord;

	/**
	* ����
	*/
	glm::vec3 tangent;

	/**
	* ������
	*/
	glm::vec3 bitangent;
};