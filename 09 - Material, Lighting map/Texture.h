#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

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

private:
	/**
	* ����ID
	*/
	unsigned int textureID = 0;

public:
	explicit Texture(const char* path);

	/**
	* ��ȡ����ID
	*/
	unsigned int GetTextureID();
};

