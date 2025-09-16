#include "TextureCube.h"

TextureCube::TextureCube(std::vector<std::string> faces)
{
	this->faces = faces;

	LoadCubemap();
}

void TextureCube::LoadCubemap()
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	stbi_set_flip_vertically_on_load(false);

	int width, height, channel;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		const char* path = faces[i].c_str();
		unsigned char* data = stbi_load(path, &width, &height, &channel, 0);

		if (data)
		{
			// 内部纹理格式设置为GL_SRGB
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			std::cout << "Load cubmap(" << GL_TEXTURE_CUBE_MAP_POSITIVE_X + i << "): " << path << " success, with: " << width << " height: " << height << " channel: " << channel << " ID: " << textureID << std::endl;
		}
		else
		{
			std::cout << "[Error] Failed to load cubemap: " << path << std::endl;
		}
	}

	stbi_set_flip_vertically_on_load(true);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

unsigned int TextureCube::GetTextureID()
{
	return textureID;
}