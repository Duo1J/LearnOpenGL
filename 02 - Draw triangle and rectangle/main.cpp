#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
 
// 顶点数据，基于标准化设备坐标(NDC)
float vertices[] = {
	0.5f, 0.5f, 0.0f,   // 右上角
	0.5f, -0.5f, 0.0f,  // 右下角
	-0.5f, -0.5f, 0.0f, // 左下角
	-0.5f, 0.5f, 0.0f   // 左上角
};

// 索引数据
unsigned int indices[] = {
	0, 1, 3, // 第一个三角形
	1, 2, 3  // 第二个三角形
};

// 顶点着色器代码
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// 片元着色器代码
const char* fragmentShaderSource = "#version 330 core\n"
"layout (location = 0) out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

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

	GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGLRenderer", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, OnSetFrameBufferSize);

	// 创建VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	// 绑定VAO
	glBindVertexArray(VAO);

	// 创建缓冲
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	// 绑定创建的缓冲到VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 写入数据到VBO
	// 这里第四个参数表示我们希望显卡如何管理给定的数据
	// GL_STATIC_DRAW 数据不会或几乎不会改变
	// GL_DYNAMIC_DRAW 数据会被改变很多
	// GL_STREAM_DRAW 数据每次绘制时都会改变
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 设置顶点属性
	// 参数1: 设置位置值为0，对于顶点着色器中的layout(location = 0)
	// 参数2: 顶点属性的大小，vec3，所以大小是3
	// 参数3: 数据类型, float
	// 参数4: 是否需要将数据标准化，会将数据映射到[-1,1]或是[0,1](取决于是否unsigned)
	// 参数5: 步长
	// 参数6: 数据偏移量
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 创建EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	// 绑定EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// 设置EBO数据
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 解绑VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// 解绑VAO
	glBindVertexArray(0);

	// 创建顶点着色器
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// 设置着色器代码，第二个参数为传入的源码字符串数量
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// 编译
	glCompileShader(vertexShader);

	// 可选，判断着色器是否编译成功
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "[Error] Vertex shader compile failed!\n" << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}

	// 创建片元着色器
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "[Error] Fragment shader compile failed!\n" << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}

	// 创建着色器程序
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	// 添加着色器
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// 链接
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "[Error] Shader program link failed!\n" << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}

	// 链接完成后即可删除着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ProcessInput(window);

		// 使用着色器程序
		glUseProgram(shaderProgram);
		// 绑定接下来要使用的VAO
		glBindVertexArray(VAO);

		// 线框模式
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// 绘制
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		// 通过EBO绘制
		// 参数2: 绘制顶点的个数
		// 参数3: 索引的数据类型
		// 偏移量
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}