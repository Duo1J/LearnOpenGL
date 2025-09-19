#include <iostream>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Define.h"
#include "BuiltinData.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Model.h"
#include "TextureCube.h"

float screenWidth = 800;
float screenHeight = 600;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera;

// 阴影深度图宽高
const unsigned int shadowWidth = 2048, shadowHeight = 2048;

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

GLFWwindow* InitEnv()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OpenGLRenderer", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		return nullptr;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	glfwSetFramebufferSizeCallback(window, OnSetFrameBufferSize);
	glfwSetCursorPosCallback(window, ProcessMouseInput);
	glfwSetScrollCallback(window, ProcessMouseWheelInput);

	glViewport(0, 0, screenWidth, screenHeight);

	return window;
}

void InitCamera()
{
	Transform cameraTransform;
	cameraTransform.position = glm::vec3(0, 0, 3);
	cameraTransform.front = glm::vec3(0, 0, -1);
	cameraTransform.up = glm::vec3(0, 1, 0);
	cameraTransform.rotate.yaw = -90;
	camera = Camera(cameraTransform);
	camera.far = 100;
}

unsigned int planeVAO;
unsigned int cubeVAO;

void RenderScene(Shader shader)
{
	shader.Use();
	shader.SetVec3("dirLight.ambient", glm::vec3(0.1f));
	shader.SetVec3("dirLight.diffuse", glm::vec3(0.9f));
	shader.SetVec3("dirLight.specular", glm::vec3(0.6f));

	glm::mat4 model = glm::mat4(1);
	shader.SetMat4("model", model);
	shader.SetVec3("color", glm::vec3(0.5f, 0.5f, 0.5f));
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	shader.SetVec3("color", glm::vec3(0, 0.7f, 0.7f));
	glBindVertexArray(cubeVAO);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	shader.SetMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-2.4, 0, -1.5));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(0.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	shader.SetMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
}

int main()
{
	GLFWwindow* window = InitEnv();
	if (window == nullptr)
	{
		EXIT;
	}

	InitCamera();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	stbi_set_flip_vertically_on_load(true);

	unsigned int planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	unsigned int cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	// ShadowMap FBO
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	// GL_DEPTH_COMPONENT
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// 防止过采样
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	// 不需要颜色缓冲
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	unsigned int screenQuadVAO, screenQuadVBO;
	glGenVertexArrays(1, &screenQuadVAO);
	glGenBuffers(1, &screenQuadVBO);
	glBindVertexArray(screenQuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadVertices), &screenQuadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glm::vec3 lightPos(4, 4, 4);
	glm::vec3 lightDir = lightPos - glm::vec3(0.0f);

	Shader planeShader("Shader/PlaneVertex.glsl", "Shader/PlaneFragment.glsl");
	Shader shadowShader("Shader/ShadowMapVertex.glsl", "Shader/ShadowMapFragment.glsl");
	Shader screenShader("Shader/ScreenVertex.glsl", "Shader/ScreenFragment.glsl");

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		ProcessKeyboardInput(window);

		// 渲染ShadowMap
		// 把光源当作相机，这里基于光源计算变换矩阵
		glm::mat4 lightProjection, lightView;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 15.0f);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		{
			shadowShader.Use();
			shadowShader.SetMat4("projection", lightProjection);
			shadowShader.SetMat4("view", lightView);

			glViewport(0, 0, shadowWidth, shadowHeight);
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);

			glCullFace(GL_FRONT);
			RenderScene(shadowShader);
			glCullFace(GL_BACK);
		}

		// 转换回来
		glViewport(0, 0, screenWidth, screenHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// 显示ShadowMap
		//{
		//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		//	glClear(GL_COLOR_BUFFER_BIT);

		//	screenShader.Use();
		//	screenShader.SetInt("screenTexture", 0);
		//	glBindVertexArray(screenQuadVAO);
		//	glDisable(GL_DEPTH_TEST);
		//	glBindTexture(GL_TEXTURE_2D, depthMap);
		//	glDrawArrays(GL_TRIANGLES, 0, 6);
		//	glEnable(GL_DEPTH_TEST);
		//}

		{
			glm::mat4 view = camera.GetViewMatrix();
			glm::mat4 projection = glm::mat4(1);
			projection = glm::perspective(glm::radians(camera.fov), screenWidth / screenHeight, camera.near, camera.far);

			planeShader.Use();
			planeShader.SetMat4("projection", projection);
			planeShader.SetMat4("view", view);
			planeShader.SetMat4("lightProjection", lightProjection);
			planeShader.SetMat4("lightView", lightView);
			planeShader.SetVec3("lightDir", lightDir);
			planeShader.SetVec3("viewPos", camera.transform.position);
			planeShader.SetInt("shadowMap", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, depthMap);

			RenderScene(planeShader);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}