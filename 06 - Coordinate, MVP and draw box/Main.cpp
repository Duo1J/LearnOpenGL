#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Shader.h"

#define EXIT glfwTerminate(); \
return -1;

// �����嶥��
float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

// ��ʱ����
unsigned int indices[] = {
	0, 1, 3, // ��һ��������
	1, 2, 3  // �ڶ���������
};

// ������Ļ���
float screenWidth = 800;
float screenHeight = 600;

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void OnSetFrameBufferSize(GLFWwindow* window, int width, int height)
{
	// �޸Ŀ��
	screenWidth = width;
	screenHeight = height;
	glViewport(0, 0, screenWidth, screenHeight);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// ��Ϊ����Ԥ����Ŀ��
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

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ȥ������ɫ
	int step = 5, curStep = 0;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, step * sizeof(float), (void*)curStep);
	glEnableVertexAttribArray(0);
	curStep += 3;
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, step * sizeof(float), (void*)(curStep * sizeof(float)));
	glEnableVertexAttribArray(1);
	curStep += 2;

	// ��ʱ����
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Shader shader("VertexShader.vs", "FragmentShader.fs");

	stbi_set_flip_vertically_on_load(true);

	int texWidth, texHeight, nrChannels;
	const char* wallTexPath = "F:/Scripts/Cpp/LearnOpenGL/learn-open-gl/Resource/T_Wall.jpg";
	unsigned char* wallTexData = stbi_load(wallTexPath, &texWidth, &texHeight, &nrChannels, 0);
	unsigned int wallTex;
	glGenTextures(1, &wallTex);
	if (wallTexData)
	{
		glBindTexture(GL_TEXTURE_2D, wallTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, wallTexData);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(wallTexData);
	}
	else
	{
		std::cout << "[Error] Failed to load texture: " << wallTexPath << std::endl;
		EXIT
	}

	const char* faceTexPath = "F:/Scripts/Cpp/LearnOpenGL/learn-open-gl/Resource/T_Face.png";
	unsigned char* faceTexData = stbi_load(faceTexPath, &texWidth, &texHeight, &nrChannels, 0);
	unsigned int faceTex;
	glGenTextures(1, &faceTex);
	if (faceTexData)
	{
		glBindTexture(GL_TEXTURE_2D, faceTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, faceTexData);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(faceTexData);
	}
	else
	{
		std::cout << "[Error] Failed to load texture: " << faceTexPath << std::endl;
		EXIT
	}

	shader.Use();
	shader.SetInt("texture1", 0);
	shader.SetInt("texture2", 1);

	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// ����GL_DEPTH_BUFFER_BIT������Ȼ���
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ProcessInput(window);

		// ����MVP����
		// Model���󣬽�����ת
		glm::mat4 model = glm::mat4(1);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1, 1, 0));
		// View�����൱����������3����λ
		glm::mat4 view = glm::mat4(1);
		view = glm::translate(view, glm::vec3(0, 0, -3));
		// Projection����
		glm::mat4 projection = glm::mat4(1);
		// ����1��FOVֵ
		// ����2����߱�
		// ����3����ƽ�����
		// ����4��Զƽ�����
		projection = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 100.0f);
		// ���ö�����ɫ���е�uniform����
		shader.SetMat4("model", glm::value_ptr(model));
		shader.SetMat4("view", glm::value_ptr(view));
		shader.SetMat4("projection", glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D, wallTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, faceTex);

		// ��ΪglDrawArrays��ʽ����
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shader.Delete();
	glfwTerminate();
	return 0;
}