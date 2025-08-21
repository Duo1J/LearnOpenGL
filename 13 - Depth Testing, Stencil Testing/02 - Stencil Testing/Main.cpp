#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Model.h"

#define EXIT glfwTerminate(); \
return -1;

float screenWidth = 800;
float screenHeight = 600;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera;

void ProcessKeyboardInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT))
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	camera.ProcessKeyboardInput(window, deltaTime);
}

void ProcessMouseInput(GLFWwindow* window, double x, double y)
{
	camera.ProcessMouseInput(window, x, y, deltaTime);
}

void ProcessMouseWheelInput(GLFWwindow* window, double x, double y)
{
	camera.ProcessMouseWheelInput(window, x, y, deltaTime);
}

void OnSetFrameBufferSize(GLFWwindow* window, int width, int height)
{
	screenWidth = width;
	screenHeight = height;
	glViewport(0, 0, screenWidth, screenHeight);
	camera.UpdateCameraVector();
}

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OpenGLRenderer", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		EXIT
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		EXIT
	}

	glfwSetFramebufferSizeCallback(window, OnSetFrameBufferSize);
	glfwSetCursorPosCallback(window, ProcessMouseInput);
	glfwSetScrollCallback(window, ProcessMouseWheelInput);

	stbi_set_flip_vertically_on_load(true);

	Transform cameraTransform;
	cameraTransform.position = glm::vec3(0, 0, 3);
	cameraTransform.front = glm::vec3(0, 0, -1);
	cameraTransform.up = glm::vec3(0, 1, 0);
	cameraTransform.rotate.yaw = -90;
	camera = Camera(cameraTransform);
	camera.far = 100;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// 启用模板测试
	glEnable(GL_STENCIL_TEST);
	// 模板和深度测试都通过后替换为参考值1
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	Shader shader("VertexShader.glsl", "FragmentShader.glsl");
	// 轮廓Shader
	Shader edgeShader("VertexShader.glsl", "EdgeFragmentShader.glsl");

	Model model("F:/Scripts/Cpp/LearnOpenGL/learn-open-gl/Resource/backpack/backpack.obj");

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// 每帧清理模板缓冲
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		ProcessKeyboardInput(window);

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::mat4(1);
		projection = glm::perspective(glm::radians(camera.fov), screenWidth / screenHeight, camera.near, camera.far);

		// !注意先Use再设值!
		shader.Use();
		shader.SetMat4("view", view);
		shader.SetMat4("projection", projection);

		shader.SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.SetVec3("viewPos", camera.transform.position);

		shader.SetFloat("near", camera.near);
		shader.SetFloat("far", camera.far);

		shader.SetVec3("dirLight.ambient", glm::vec3(0.1f));
		shader.SetVec3("dirLight.diffuse", glm::vec3(0.9f));
		shader.SetVec3("dirLight.specular", glm::vec3(0.6f));
		shader.SetVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		shader.SetFloat("material.shininess", 32.0f);

		for (int i = 0; i < 4; i++)
		{
			shader.SetVec3("pointLight[" + std::to_string(i) + "].ambient", glm::vec3(0.05f));
			shader.SetVec3("pointLight[" + std::to_string(i) + "].diffuse", glm::vec3(0.8f));
			shader.SetVec3("pointLight[" + std::to_string(i) + "].specular", glm::vec3(1.0f));
			shader.SetVec3("pointLight[" + std::to_string(i) + "].position", glm::vec3(pointLightPositions[i]));
			shader.SetFloat("pointLight[" + std::to_string(i) + "].constant", 1.0f);
			shader.SetFloat("pointLight[" + std::to_string(i) + "].diffuse", 0.09f);
			shader.SetFloat("pointLight[" + std::to_string(i) + "].quadratic", 0.032f);
		}

		shader.SetVec3("spotLight.ambient", glm::vec3(0));
		shader.SetVec3("spotLight.diffuse", glm::vec3(1));
		shader.SetVec3("spotLight.specular", glm::vec3(1));

		shader.SetVec3("spotLight.position", camera.transform.position);
		shader.SetVec3("spotLight.direction", camera.transform.front);

		shader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		shader.SetFloat("spotLight.cutOffOuter", glm::cos(glm::radians(17.5f)));
		shader.SetFloat("spotLight.constant", 1.0f);
		shader.SetFloat("spotLight.linear", 0.09f);
		shader.SetFloat("spotLight.quadratic", 0.032f);

		edgeShader.Use();
		edgeShader.SetMat4("view", view);
		edgeShader.SetMat4("projection", projection);

		// 总是通过模板测试，并写入1
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		// 绘制原物体
		shader.Use();
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		shader.SetMat4("model", modelMatrix);
		model.Draw(shader);

		// 不等于1时通过
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		// 禁止写入模板缓冲
		glStencilMask(0x00);
		// 禁止深度测试
		glDisable(GL_DEPTH_TEST);

		// 放大物体
		edgeShader.Use();
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.01f, 1.01f, 1.01f));
		edgeShader.SetMat4("model", modelMatrix);
		model.Draw(edgeShader);

		// 开启模板写入
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		// 开启深度测试
		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shader.Delete();
	edgeShader.Delete();
	glfwTerminate();
	return 0;
}