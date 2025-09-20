#include "Texture.h"
#include <assert.h>

Texture::Texture(const char* path, TextureType type, unsigned int wrap)
{
	this->path = path;
	this->type = type;

	// 仅对漫反射贴图进行Gamma矫正
	bool gammaCorrection = false;
	if (this->type == TextureType::DIFFUSE)
	{
		gammaCorrection = true;
	}

	unsigned char* textureData = stbi_load(path, &width, &height, &channel, 0);
	glGenTextures(1, &textureID);

	if (textureData)
	{
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int internalFormat, dataFormat;
		if (channel == 4)
		{
			internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
		}
		else if (channel == 3)
		{
			internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
		}
		else if (channel == 2)
		{
			internalFormat = dataFormat = GL_RG;
		}
		else
		{
			internalFormat = dataFormat = GL_RED;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, textureData);

		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(textureData);
		std::cout << "Load texture: " << path << " success, with: " << width << " height: " << height << " channel: " << channel << " ID: " << textureID << std::endl;
	}
	else
	{
		std::cout << "[Error] Failed to load texture: " << path << std::endl;
	}
}

unsigned int Texture::GetTextureID()
{
	return textureID;
}

std::string Texture::GetTypeName()
{
	if (type == TextureType::DIFFUSE)
	{
		return "tex_diffuse";
	}
	else if (type == TextureType::SPECULAR)
	{
		return "tex_specular";
	}
	else if (type == TextureType::NORMAL)
	{
		return "tex_normal";
	}
	else
	{
		assert(false);
		return "";
	}
}
