#pragma once

#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "Transform.h"

/**
* �����
*/
class Camera
{
public:
	/**
	* �任
	*/
	Transform transform;

	/**
	* FOV
	*/
	float fov = 45;

	/**
	* �ƶ��ٶ�
	*/
	float moveSpeed = 2.5f;

	/**
	* ��ת������
	*/
	float rotateSensity = 0.05f;

	/**
	* ��ƽ��
	*/
	float near = 0.01f;

	/**
	* Զƽ��
	*/
	float far = 100;

public:
	Camera();
	Camera(Transform& _transform);

	/**
	* ��ȡ�۲����
	*/
	glm::mat4 GetViewMatrix();

	/**
	* �����������
	*/
	void ProcessKeyboardInput(GLFWwindow* window, float deltaTime);

	/**
	* �����������
	*/
	void ProcessMouseInput(GLFWwindow* window, double x, double y, float deltaTime);

	/**
	* �����������
	*/
	void ProcessMouseWheelInput(GLFWwindow* window, double x, double y, float deltaTime);

	/**
	* �����������
	*/
	void UpdateCameraVector();
};

