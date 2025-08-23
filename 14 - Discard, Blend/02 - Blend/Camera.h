#pragma once

#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "Transform.h"

/**
* 摄像机
*/
class Camera
{
public:
	/**
	* 变换
	*/
	Transform transform;

	/**
	* FOV
	*/
	float fov = 45;

	/**
	* 移动速度
	*/
	float moveSpeed = 2.5f;

	/**
	* 旋转灵敏度
	*/
	float rotateSensity = 0.05f;

	/**
	* 近平面
	*/
	float near = 0.01f;

	/**
	* 远平面
	*/
	float far = 100;

public:
	Camera();
	Camera(Transform& _transform);

	/**
	* 获取观察矩阵
	*/
	glm::mat4 GetViewMatrix();

	/**
	* 处理键盘输入
	*/
	void ProcessKeyboardInput(GLFWwindow* window, float deltaTime);

	/**
	* 处理鼠标输入
	*/
	void ProcessMouseInput(GLFWwindow* window, double x, double y, float deltaTime);

	/**
	* 处理滚轮输入
	*/
	void ProcessMouseWheelInput(GLFWwindow* window, double x, double y, float deltaTime);

	/**
	* 更新相机向量
	*/
	void UpdateCameraVector();
};

