#include <iostream>

// 注意顺序，glad中有依赖检查
// 顺序错误编译会报错 #error:  OpenGL header already included, remove this include, glad already provides it
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// 输入处理
void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}
}

// 窗口大小改变回调
void OnSetFrameBufferSize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	// glfw初始化
	glfwInit();
	// glfw设置OpenGL版本为3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfw设置OpenGL为核心(Core)模式
	// 核心模式相较于立即渲染模式(Immediate mode，也就是固定渲染管线，现已废弃)，拥有更高的灵活性
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建一个窗口对象
	GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGLRenderer", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// 设置为当前上下文
	glfwMakeContextCurrent(window);

	// 初始化glad加载器
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 注册窗口大小改变回调
	glfwSetFramebufferSizeCallback(window, OnSetFrameBufferSize);

	// 主循环
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 输入处理
		ProcessInput(window);

		// ...之后会做其他事情

		// 双缓冲切换
		glfwSwapBuffers(window);
		// 检查事件触发，如窗口大小改变，调用对应回调函数
		glfwPollEvents();
	}

	// 资源清理
	glfwTerminate();
	return 0;
}