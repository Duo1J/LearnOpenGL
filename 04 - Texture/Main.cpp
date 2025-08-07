#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "Shader.h"

#define EXIT glfwTerminate(); \
return -1;

float vertices[] = {
	//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
};

unsigned int indices[] = {
	0, 1, 3, // 第一个三角形
	1, 2, 3  // 第二个三角形
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

	// 纹理
	// OpenGL要求图片y轴的0坐标是在底部，而图片y轴0坐标往往是在顶部，在加载前需要设置翻转
	stbi_set_flip_vertically_on_load(true);

	// 加载纹理，并获取宽高和颜色通道数
	int texWidth, texHeight, nrChannels;
	const char* wallTexPath = "F:/Scripts/Cpp/LearnOpenGL/learn-open-gl/Resource/T_Wall.jpg";
	unsigned char* wallTexData = stbi_load(wallTexPath, &texWidth, &texHeight, &nrChannels, 0);
	unsigned int wallTex;
	// 创建纹理
	glGenTextures(1, &wallTex);
	if (wallTexData)
	{
		// 绑定纹理
		glBindTexture(GL_TEXTURE_2D, wallTex);
		// 设置纹理参数
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// 传入纹理数据
		// 参数1：纹理类型
		// 参数2：Mipmap级别
		// 参数3：纹理存储格式
		// 参数4、5：宽高
		// 参数6: 历史遗留原因，总是0
		// 参数7、8：源图格式和数据类型，我们使用RGB值加载图像并存储为char(byte)数组
		// 参数9：纹理数据
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, wallTexData);
		// 生成Mipmap
		// 要使用Mipmap，我们需要不断递增`glTexImage2D`的第二个参数来调用，或是直接使用`glGenerateMipmap`
		glGenerateMipmap(GL_TEXTURE_2D);
		// 释放
		stbi_image_free(wallTexData);
	}
	else
	{
		std::cout << "[Error] Failed to load texture: " << wallTexPath << std::endl;
		EXIT
	}

	// 加载第二张纹理
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
		// 带Alpha通道
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
		// 设置uniform采样器对应的纹理单元
		shader.SetInt("texture1", 0);
		shader.SetInt("texture2", 1);

		glBindVertexArray(VAO);
		// 激活纹理单元并绑定纹理
		// GL_TEXTURE0默认激活，这一步可以省略
		// glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, wallTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, faceTex);

		// 换回glDrawElements
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	shader.Delete();
	glfwTerminate();
	return 0;
}