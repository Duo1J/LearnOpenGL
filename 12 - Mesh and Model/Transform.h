#pragma once

#include <glm.hpp>
#include "EulerAngle.h"

/**
* 变换
*/
struct Transform
{
public:
	/**
	* 位置
	*/
	glm::vec3 position = glm::vec3(0, 0, 0);

	/**
	* 向前
	*/
	glm::vec3 front = glm::vec3(0, 0, 0);

	/**
	* 向上
	*/
	glm::vec3 up = glm::vec3(0, 0, 0);

	/**
	* 向右
	*/
	glm::vec3 right = glm::vec3(0, 0, 0);

	/**
	* 旋转欧拉角
	*/
	EulerAngle rotate;
};