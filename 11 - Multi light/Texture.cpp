#include "Texture.h"

Texture::Texture(const char* path)
{
	unsigned char* textureData = stbi_load(path, &width, &height, &channel, 0);
	glGenTextures(1, &textureID);

	if (textureData)
	{
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (channel == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(textureData);
		std::cout << "Load texture: " << path << " success, with: " << width << " height: " << height << " channel: " << channel << std::endl;
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
