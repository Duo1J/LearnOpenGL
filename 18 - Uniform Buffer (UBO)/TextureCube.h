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
	* ��ȡ����ID
	*/
	unsigned int GetTextureID();

private:
	/**
	* ����ID
	*/
	unsigned int textureID = 0;

	/**
	* ����·��
	* ˳�� Right - Left - Top - Bottom - Front - Back
	*/
	std::vector<std::string> faces;

	/**
	* ����
	*/
	void LoadCubemap();
};

