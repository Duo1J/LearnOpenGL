#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

/**
* 纹理类
*/
class Texture
{
public:
	/**
	* 纹理宽度
	*/
	int width = 0;

	/**
	* 纹理高度
	*/
	int height = 0;

	/**
	* 纹理通道数
	*/
	int channel = 0;

private:
	/**
	* 纹理ID
	*/
	unsigned int textureID = 0;

public:
	explicit Texture(const char* path);

	/**
	* 获取纹理ID
	*/
	unsigned int GetTextureID();
};

