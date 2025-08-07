#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "Shader.h"

#define EXIT glfwTerminate(); \
return -1;

float vertices[] = {
	//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
};

unsigned int indices[] = {
	0, 1, 3, // ��һ��������
	1, 2, 3  // �ڶ���������
};

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void OnSetFrameBufferSize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGLRenderer", NULL, NULL);
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

	int step = 8, curStep = 0;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, step * sizeof(float), (void*)curStep);
	glEnableVertexAttribArray(0);
	curStep += 3;
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, step * sizeof(float), (void*)(curStep * sizeof(float)));
	glEnableVertexAttribArray(1);
	curStep += 3;
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, step * sizeof(float), (void*)(curStep * sizeof(float)));
	glEnableVertexAttribArray(2);
	curStep += 2;

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Shader shader("VertexShader.vs", "FragmentShader.fs");

	// ����
	// OpenGLҪ��ͼƬy���0�������ڵײ�����ͼƬy��0�����������ڶ������ڼ���ǰ��Ҫ���÷�ת
	stbi_set_flip_vertically_on_load(true);

	// ������������ȡ��ߺ���ɫͨ����
	int texWidth, texHeight, nrChannels;
	const char* wallTexPath = "F:/Scripts/Cpp/LearnOpenGL/learn-open-gl/Resource/T_Wall.jpg";
	unsigned char* wallTexData = stbi_load(wallTexPath, &texWidth, &texHeight, &nrChannels, 0);
	unsigned int wallTex;
	// ��������
	glGenTextures(1, &wallTex);
	if (wallTexData)
	{
		// ������
		glBindTexture(GL_TEXTURE_2D, wallTex);
		// �����������
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// ������������
		// ����1����������
		// ����2��Mipmap����
		// ����3������洢��ʽ
		// ����4��5�����
		// ����6: ��ʷ����ԭ������0
		// ����7��8��Դͼ��ʽ���������ͣ�����ʹ��RGBֵ����ͼ�񲢴洢Ϊchar(byte)����
		// ����9����������
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, wallTexData);
		// ����Mipmap
		// Ҫʹ��Mipmap��������Ҫ���ϵ���`glTexImage2D`�ĵڶ������������ã�����ֱ��ʹ��`glGenerateMipmap`
		glGenerateMipmap(GL_TEXTURE_2D);
		// �ͷ�
		stbi_image_free(wallTexData);
	}
	else
	{
		std::cout << "[Error] Failed to load texture: " << wallTexPath << std::endl;
		EXIT
	}

	// ���صڶ�������
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
		// ��Alphaͨ��
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, faceTexData);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(faceTexData);
	}
	else
	{
		std::cout << "[Error] Failed to load texture: " << faceTexPath << std::endl;
		EXIT
	}

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ProcessInput(window);

		shader.Use();
		// ����uniform��������Ӧ������Ԫ
		shader.SetInt("texture1", 0);
		shader.SetInt("texture2", 1);

		glBindVertexArray(VAO);
		// ��������Ԫ��������
		// GL_TEXTURE0Ĭ�ϼ����һ������ʡ��
		// glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, wallTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, faceTex);

		// ����glDrawElements
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shader.Delete();
	glfwTerminate();
	return 0;
}