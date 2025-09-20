#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

/**
* ��������
*/
enum class TextureType
{
	/**
	* ��������ͼ
	*/
	DIFFUSE,

	/**
	* ���淴����ͼ
	*/
	SPECULAR,

	/**
	* ������ͼ
	*/
	NORMAL,
};

/**
* ������
*/
class Texture
{
public:
	/**
	* ������
	*/
	int width = 0;

	/**
	* ����߶�
	*/
	int height = 0;

	/**
	* ����ͨ����
	*/
	int channel = 0;

	/**
	* ��������
	*/
	TextureType type;

	/**
	* ����·��
	*/
	std::string path;

private:
	/**
	* ����ID
	*/
	unsigned int textureID = 0;

public:
	explicit Texture(const char* path, TextureType type, unsigned int wrap = GL_REPEAT);

	/**
	* ��ȡ����ID
	*/
	unsigned int GetTextureID();

	/**
	* ��ȡ�������͵��ֶ�����
	*/
	std::string GetTypeName();
};

