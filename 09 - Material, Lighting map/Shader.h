#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>;
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

/**
* ��ɫ��
*/
class Shader
{
public:
	/**
	* ��ɫ������ID
	*/
	unsigned int ProgramID;

	/**
	* @param vertexPath: ������ɫ��·��
	* @param fragmentPath: Ƭ����ɫ��·��
	*/
	Shader(const char* vertexPath, const char* fragmentPath);

	/**
	* ʹ�������ɫ������
	*/
	void Use();

	/**
	* ���������ɫ������
	*/
	void Delete();

	/**
	* ����bool����uniformֵ
	*/
	void SetBool(const std::string& name, bool value) const;

	/**
	* ����int����uniformֵ
	*/
	void SetInt(const std::string& name, int value) const;

	/**
	* ����float����uniformֵ
	*/
	void SetFloat(const std::string& name, float value) const;

	/**
	* ����mat4����uniformֵ
	*/
	void SetMat4(const std::string& name, float* value) const;

	/**
	* ����mat4����uniformֵ
	*/
	void SetMat4(const std::string& name, glm::mat4 value) const;

	/**
	* ����vec3����uniformֵ
	*/
	void SetVec3(const std::string& name, glm::vec3 value) const;
};
