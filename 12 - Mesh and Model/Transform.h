#pragma once

#include <glm.hpp>
#include "EulerAngle.h"

/**
* �任
*/
struct Transform
{
public:
	/**
	* λ��
	*/
	glm::vec3 position = glm::vec3(0, 0, 0);

	/**
	* ��ǰ
	*/
	glm::vec3 front = glm::vec3(0, 0, 0);

	/**
	* ����
	*/
	glm::vec3 up = glm::vec3(0, 0, 0);

	/**
	* ����
	*/
	glm::vec3 right = glm::vec3(0, 0, 0);

	/**
	* ��תŷ����
	*/
	EulerAngle rotate;
};