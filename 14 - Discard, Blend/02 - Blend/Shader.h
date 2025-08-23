#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>;
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

/**
* 着色器
*/
class Shader
{
public:
	/**
	* 着色器程序ID
	*/
	unsigned int ProgramID;

	/**
	* @param vertexPath: 顶点着色器路径
	* @param fragmentPath: 片段着色器路径
	*/
	Shader(const char* vertexPath, const char* fragmentPath);

	/**
	* 使用这个着色器程序
	*/
	void Use();

	/**
	* 销毁这个着色器程序
	*/
	void Delete();

	/**
	* 设置bool类型uniform值
	*/
	void SetBool(const std::string& name, bool value) const;

	/**
	* 设置int类型uniform值
	*/
	void SetInt(const std::string& name, int value) const;

	/**
	* 设置float类型uniform值
	*/
	void SetFloat(const std::string& name, float value) const;

	/**
	* 设置mat4类型uniform值
	*/
	void SetMat4(const std::string& name, float* value) const;

	/**
	* 设置mat4类型uniform值
	*/
	void SetMat4(const std::string& name, glm::mat4 value) const;

	/**
	* 设置vec3类型uniform值
	*/
	void SetVec3(const std::string& name, glm::vec3 value) const;
};
