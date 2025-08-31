#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

class TextureCube
{
public:
	explicit TextureCube(std::vector<std::string> faces);

	/**
	* 获取纹理ID
	*/
	unsigned int GetTextureID();

private:
	/**
	* 纹理ID
	*/
	unsigned int textureID = 0;

	/**
	* 六面路径
	* 顺序 Right - Left - Top - Bottom - Front - Back
	*/
	std::vector<std::string> faces;

	/**
	* 加载
	*/
	void LoadCubemap();
};

