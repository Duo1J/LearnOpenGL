#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

/**
* 纹理类型
*/
enum class TextureType
{
	/**
	* 漫反射贴图
	*/
	DIFFUSE,

	/**
	* 镜面反射贴图
	*/
	SPECULAR,

	/**
	* 法线贴图
	*/
	NORMAL,
};

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

	/**
	* 纹理类型
	*/
	TextureType type;

	/**
	* 纹理路径
	*/
	std::string path;

private:
	/**
	* 纹理ID
	*/
	unsigned int textureID = 0;

public:
	explicit Texture(const char* path, TextureType type, unsigned int wrap = GL_REPEAT);

	/**
	* 获取纹理ID
	*/
	unsigned int GetTextureID();

	/**
	* 获取纹理类型的字段名称
	*/
	std::string GetTypeName();
};

