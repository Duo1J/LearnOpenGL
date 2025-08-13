#include "Camera.h"
#include <gtc/matrix_transform.hpp>

Camera::Camera()
{
	transform.front = glm::vec3(0, 0, 1);
	transform.up = glm::vec3(0, 1, 0);

	UpdateCameraVector();
}

Camera::Camera(Transform& _transform)
{
	transform = _transform;
	UpdateCameraVector();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(transform.position, transform.position + transform.front, transform.up);
}

void Camera::ProcessKeyboardInput(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		transform.position += transform.front * moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		transform.position -= transform.front * moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		transform.position -= glm::normalize(glm::cross(transform.front, transform.up)) * moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		transform.position += glm::normalize(glm::cross(transform.front, transform.up)) * moveSpeed * deltaTime;
	}
}

void Camera::ProcessMouseInput(GLFWwindow* window, double x, double y, float deltaTime)
{
	static bool firstMouseInput = true;
	static float lastX = 0, lastY = 0;
	if (firstMouseInput)
	{
		firstMouseInput = false;
		lastX = x;
		lastY = y;
	}

	float xOffset = x - lastX;
	float yOffset = lastY - y;
	lastX = x;
	lastY = y;

	xOffset *= rotateSensity;
	yOffset *= rotateSensity;

	transform.rotate.yaw += xOffset;
	transform.rotate.pitch += yOffset;

	UpdateCameraVector();
}

void Camera::ProcessMouseWheelInput(GLFWwindow* window, double x, double y, float deltaTime)
{
	fov += -y;
	fov = glm::clamp(fov, 1.0f, 90.0f);
}

void Camera::UpdateCameraVector()
{
	transform.rotate.pitch = glm::clamp(transform.rotate.pitch, -89.0f, 89.0f);

	transform.front.x = cos(glm::radians(transform.rotate.yaw)) * cos(glm::radians(transform.rotate.pitch));
	transform.front.y = sin(glm::radians(transform.rotate.pitch));
	transform.front.z = sin(glm::radians(transform.rotate.yaw)) * cos(glm::radians(transform.rotate.pitch));
	transform.front = glm::normalize(transform.front);
}
