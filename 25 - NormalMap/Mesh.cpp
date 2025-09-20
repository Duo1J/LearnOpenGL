#include "Mesh.h"
#include <glad/glad.h>
#include <assert.h>

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures)
{
	vertices = _vertices;
	indices = _indices;
	textures = _textures;

	CreateBuffer();
}

void Mesh::Draw(const Shader& shader, int instanceCnt)
{
	// ������������
	unsigned int diffuseNum = 0;
	unsigned int specularNum = 0;
	unsigned int normalNum = 0;

	for (int i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		// �������Ӧ���õ��ڼ���������
		std::string number;
		Texture texture = textures[i];
		if (texture.type == TextureType::DIFFUSE)
		{
			number = std::to_string(++diffuseNum);
		}
		else if (texture.type == TextureType::SPECULAR)
		{
			number = std::to_string(++specularNum);
		}
		else if (texture.type == TextureType::NORMAL)
		{
			number = std::to_string(++normalNum);
		}
		else
		{
			assert(false);
		}

		// ��ɫ���е�������
		std::string name = "material." + texture.GetTypeName() + number;
		shader.SetInt(name, i);
		glBindTexture(GL_TEXTURE_2D, texture.GetTextureID());
	}

	// �󶨻���
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	BindVAO();

	//����
	if (instanceCnt > 1)
		glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instanceCnt);
	else
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	// ���
	UnBindVAO();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::BindVAO()
{
	glBindVertexArray(VAO);
}

void Mesh::UnBindVAO()
{
	glBindVertexArray(0);
}

void Mesh::CreateBuffer()
{
	// ��������
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// ��VAO
	BindVAO();

	// �󶨶�������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// ����������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// ���ö�������
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

	// �ⲿ��ʱ����Instance Offset��������

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

	// ���
	UnBindVAO();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
