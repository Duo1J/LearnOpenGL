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

unsigned int framebufferTex;
unsigned int rbo;

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

	glBindTexture(GL_TEXTURE_2D, framebufferTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
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

	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glGenTextures(1, &framebufferTex);
	glBindTexture(GL_TEXTURE_2D, framebufferTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTex, 0);
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "[Error] Framebuffer is not complete" << std::endl;
		EXIT
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Shader shader("Shader/VertexShader.glsl", "Shader/FragmentShader.glsl");
	Shader reflectShader("Shader/VertexShader.glsl", "Shader/ReflectFragment.glsl");
	Shader refractShader("Shader/VertexShader.glsl", "Shader/RefractFragment.glsl");
	Shader edgeShader("Shader/VertexShader.glsl", "Shader/EdgeFragmentShader.glsl");
	Shader grassShader("Shader/VertexShaderNoNormal.glsl", "Shader/GrassFragment.glsl");
	Shader windowShader("Shader/VertexShaderNoNormal.glsl", "Shader/WindowFragment.glsl");
	Shader screenShader("Shader/ScreenVertex.glsl", "Shader/ScreenFragment.glsl");
	Shader skyboxShader("Shader/SkyboxVertex.glsl", "Shader/SkyboxFragment.glsl");
	// 法线可视化Shader
	Shader normalVisualShader("Shader/NormalVisualVertex.glsl", "Shader/GreenFragment.glsl", "Shader/NormalVisualGeo.glsl");

	Texture grassTex("F:/Scripts/Cpp/LearnOpenGL/learn-open-gl/Resource/T_Grass.png", TextureType::DIFFUSE, GL_CLAMP_TO_EDGE);
	Texture windowTex("F:/Scripts/Cpp/LearnOpenGL/learn-open-gl/Resource/T_Window.png", TextureType::DIFFUSE, GL_CLAMP_TO_EDGE);
	TextureCube skyboxCubemap(skyboxFaces);

	Model model("F:/Scripts/Cpp/LearnOpenGL/learn-open-gl/Resource/backpack/backpack.obj");

	unsigned int uboMaterices;
	glGenBuffers(1, &uboMaterices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMaterices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMaterices, 0, 2 * sizeof(glm::mat4));

	BindMatericesBlock(bag, shader, 0);
	BindMatericesBlock(reflect, reflectShader, 0);
	BindMatericesBlock(refract, refractShader, 0);
	BindMatericesBlock(edge, edgeShader, 0);
	BindMatericesBlock(grass, grassShader, 0);
	BindMatericesBlock(window, windowShader, 0);
	BindMatericesBlock(normalVisual, normalVisualShader, 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		ProcessKeyboardInput(window);

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::mat4(1);
		projection = glm::perspective(glm::radians(camera.fov), screenWidth / screenHeight, camera.near, camera.far);

		glBindBuffer(GL_UNIFORM_BUFFER, uboMaterices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		grassShader.Use();
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

		shader.SetFloat("time", glfwGetTime());

		edgeShader.Use();

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		shader.Use();
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		shader.SetMat4("model", modelMatrix);
		model.Draw(shader);

		// 法线可视化
		normalVisualShader.Use();
		normalVisualShader.SetMat4("model", modelMatrix);
		model.Draw(normalVisualShader);

		modelMatrix = glm::translate(modelMatrix, glm::vec3(-5.0f, 0.0f, 0.0f));
		reflectShader.Use();
		reflectShader.SetMat4("model", modelMatrix);
		reflectShader.SetVec3("cameraPos", camera.transform.position);
		reflectShader.SetInt("skybox", 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubemap.GetTextureID());
		model.Draw(shader);

		modelMatrix = glm::translate(modelMatrix, glm::vec3(-5.0f, 0.0f, 0.0f));
		refractShader.Use();
		refractShader.SetMat4("model", modelMatrix);
		refractShader.SetVec3("cameraPos", camera.transform.position);
		refractShader.SetInt("skybox", 0);
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

		glDepthFunc(GL_LEQUAL);
		skyboxShader.Use();
		skyboxShader.SetInt("skybox", 0);
		skyboxShader.SetMat4("view", glm::mat4(glm::mat3(view)));
		skyboxShader.SetMat4("projection", projection);
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubemap.GetTextureID());
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		// 半透明
		std::map<float, glm::vec3> sortedWindowPositions;
		for (unsigned int i = 0; i < windowPositions.size(); ++i)
		{
			float distance = glm::length(camera.transform.position - windowPositions[i]);
			sortedWindowPositions[distance] = windowPositions[i];
		}

		windowShader.Use();
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

		// 后处理
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		screenShader.Use();
		screenShader.SetInt("screenTexture", 0);
		glBindVertexArray(screenQuadVAO);
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