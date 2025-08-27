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

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

float quadVertices[] = {
	// positions         // texture Coords
	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
	1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};

// 屏幕Quad顶点
float screenQuadVertices[] = {
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
};

std::vector<glm::vec3> grassPositions
{
	glm::vec3(1.5f, 0.0f, 0.51f),
	glm::vec3(0.0f, 0.0f, 0.7f),
};

std::vector<glm::vec3> windowPositions
{
	glm::vec3(0.0f, 0.0f, 2.7f),
	glm::vec3(-0.2f, 0.0f, 1.71f),
	glm::vec3(0.2f, 0.5f, 0.51f),
};

int main()
{
	GLFWwindow* window = InitEnv();
	if (window == nullptr)
	{
		EXIT;
	}

	InitCamera();

	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 屏幕Quad顶点数据
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

	// 帧缓冲
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// 纹理附件
	unsigned int framebufferTex;
	glGenTextures(1, &framebufferTex);
	glBindTexture(GL_TEXTURE_2D, framebufferTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	// 附加纹理附件
	// 参数1：帧缓冲的目标
	// 参数2：附加的附件类型
	// 参数3：附加的纹理类型
	// 参数4：附加的纹理
	// 参数5：mipmap级别
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTex, 0);

	// 渲染缓冲附件
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	// 附加渲染缓冲附件
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// 检查帧缓冲完整性
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "[Error] Framebuffer is not complete" << std::endl;
		EXIT
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Shader shader("Shader/VertexShader.glsl", "Shader/FragmentShader.glsl");
	Shader edgeShader("Shader/VertexShader.glsl", "Shader/EdgeFragmentShader.glsl");
	Shader grassShader("Shader/VertexShaderNoNormal.glsl", "Shader/GrassFragment.glsl");
	Shader windowShader("Shader/VertexShaderNoNormal.glsl", "Shader/WindowFragment.glsl");
	// 屏幕Shader
	Shader screenShader("Shader/ScreenVertex.glsl", "Shader/ScreenFragment.glsl");

	Texture grassTex("F:/Scripts/Cpp/LearnOpenGL/learn-open-gl/Resource/T_Grass.png", TextureType::DIFFUSE, GL_CLAMP_TO_EDGE);
	Texture windowTex("F:/Scripts/Cpp/LearnOpenGL/learn-open-gl/Resource/T_Window.png", TextureType::DIFFUSE, GL_CLAMP_TO_EDGE);

	Model model("F:/Scripts/Cpp/LearnOpenGL/learn-open-gl/Resource/backpack/backpack.obj");

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// 绑定我们创建的帧缓冲
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		ProcessKeyboardInput(window);

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::mat4(1);
		projection = glm::perspective(glm::radians(camera.fov), screenWidth / screenHeight, camera.near, camera.far);

		grassShader.Use();
		grassShader.SetMat4("view", view);
		grassShader.SetMat4("projection", projection);
		grassShader.SetInt(grassTex.GetTypeName() + '1', 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, grassTex.GetTextureID());
		glDisable(GL_CULL_FACE);
		for (int i = 0; i < grassPositions.size(); ++i)
		{
			glm::mat4 model = glm::mat4(1);
			model = glm::translate(model, grassPositions[i]);
			grassShader.SetMat4("model", model);
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
		glEnable(GL_CULL_FACE);

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

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		shader.Use();
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		shader.SetMat4("model", modelMatrix);
		model.Draw(shader);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		edgeShader.Use();
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.01f, 1.01f, 1.01f));
		edgeShader.SetMat4("model", modelMatrix);
		model.Draw(edgeShader);

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);

		std::map<float, glm::vec3> sortedWindowPositions;
		for (unsigned int i = 0; i < windowPositions.size(); ++i)
		{
			float distance = glm::length(camera.transform.position - windowPositions[i]);
			sortedWindowPositions[distance] = windowPositions[i];
		}

		windowShader.Use();
		windowShader.SetMat4("view", view);
		windowShader.SetMat4("projection", projection);
		windowShader.SetInt(windowTex.GetTypeName() + '1', 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, windowTex.GetTextureID());
		glDisable(GL_CULL_FACE);
		for (std::map<float, glm::vec3>::reverse_iterator it = sortedWindowPositions.rbegin();
			it != sortedWindowPositions.rend();
			++it)
		{
			glm::mat4 model = glm::mat4(1);
			model = glm::translate(model, it->second);
			windowShader.SetMat4("model", model);
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
		glEnable(GL_CULL_FACE);

		// 绑定默认的帧缓冲
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 绘制屏幕矩形
		screenShader.Use();
		screenShader.SetInt("screenTexture", 0);
		glBindVertexArray(screenQuadVAO);
		// 不需要深度测试
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, framebufferTex);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shader.Delete();
	edgeShader.Delete();
	glfwTerminate();
	return 0;
}