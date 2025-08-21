#include "Texture.h"
#include <assert.h>

Texture::Texture(const char* path, TextureType type)
{
	this->path = path;
	this->type = type;

	unsigned char* textureData = stbi_load(path, &width, &height, &channel, 0);
	glGenTextures(1, &textureID);

	if (textureData)
	{
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int ch;
		if (channel == 4)
		{
			ch = GL_RGBA;
		}
		else if (channel == 3)
		{
			ch = GL_RGB;
		}
		else if (channel == 2)
		{
			ch = GL_RG;
		}
		else
		{
			ch = GL_RED;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, ch, width, height, 0, ch, GL_UNSIGNED_BYTE, textureData);

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
	else
	{
		assert(false);
		return "";
	}
}
