#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geoPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::string geoCode;

	try
	{
		if (vertexPath != nullptr)
		{
			std::ifstream vertexShaderFile;
			vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			vertexShaderFile.open(vertexPath);
			std::stringstream vShaderStream;
			vShaderStream << vertexShaderFile.rdbuf();
			vertexShaderFile.close();
			vertexCode = vShaderStream.str();
		}

		if (fragmentPath != nullptr)
		{
			std::ifstream fragmentShaderFile;
			fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fragmentShaderFile.open(fragmentPath);
			std::stringstream fShaderStream;
			fShaderStream << fragmentShaderFile.rdbuf();
			fragmentShaderFile.close();
			fragmentCode = fShaderStream.str();
		}

		if (geoPath != nullptr)
		{
			std::ifstream geoShaderFile;
			geoShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			geoShaderFile.open(geoPath);
			std::stringstream gShaderStream;
			gShaderStream << geoShaderFile.rdbuf();
			geoShaderFile.close();
			geoCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "[Error] Failed to read shader file" << std::endl;
	}

	ProgramID = glCreateProgram();

	int success;
	char infoLog[512];

	// 顶点着色器
	unsigned int vertexShader = 0;
	if (!vertexCode.empty())
	{
		const char* vertexShaderCode = vertexCode.c_str();
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "[Error] Vertex shader compile failed!\n" << infoLog << std::endl;
		};
		glAttachShader(ProgramID, vertexShader);
	}

	// 片段着色器
	unsigned int fragmentShader = 0;
	if (!fragmentCode.empty())
	{
		const char* fragmentShaderCode = fragmentCode.c_str();
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "[Error] Fragment shader compile failed!\n" << infoLog << std::endl;
		}
		glAttachShader(ProgramID, fragmentShader);
	}

	// 几何着色器
	unsigned int geoShader = 0;
	if (!geoCode.empty())
	{
		const char* geoShaderCode = geoCode.c_str();
		geoShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geoShader, 1, &geoShaderCode, NULL);
		glCompileShader(geoShader);
		glGetShaderiv(geoShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(geoShader, 512, NULL, infoLog);
			std::cout << "[Error] Geo shader compile failed!\n" << infoLog << std::endl;
		}
		glAttachShader(ProgramID, geoShader);
	}

	glLinkProgram(ProgramID);
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ProgramID, 512, NULL, infoLog);
		std::cout << "[Error] Shader program link failed!\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(geoShader);
}

void Shader::Use()
{
	glUseProgram(ProgramID);
}

void Shader::Delete()
{
	glDeleteProgram(ProgramID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ProgramID, name.c_str()), value);
}

void Shader::SetMat4(const std::string& name, float* value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::SetMat4(const std::string& name, glm::mat4 value) const
{
	SetMat4(name, glm::value_ptr(value));
}

void Shader::SetVec3(const std::string& name, glm::vec3 value) const
{
	glUniform3f(glGetUniformLocation(ProgramID, name.c_str()), value.x, value.y, value.z);
}
