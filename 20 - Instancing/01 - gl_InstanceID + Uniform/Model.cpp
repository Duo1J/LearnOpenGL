#include "Model.h"

Model::Model(const char* path)
{
	LoadModel(path);
}

void Model::Draw(Shader shader, int instanceCnt)
{
	for (unsigned int i = 0; i < meshes.size(); ++i)
	{
		meshes[i].Draw(shader, instanceCnt);
	}
}

void Model::LoadModel(std::string path)
{
	Assimp::Importer importer;
	// aiProcess_Triangulate: ���ģ�Ͳ���ȫ����������ɣ���Ҫת��Ϊ������
	// aiProcess_FlipUVs: OpenGL����Ҫ��תUV
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	// AI_SCENE_FLAGS_INCOMPLETE �Ƿ�δ������ȫ
	if (!scene || scene->mFlags * AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "[Error] Assimp: " << importer.GetErrorString() << std::endl;
		return;
	}

	// ��ȡ�����ļ��ĸ�·��
	directory = path.substr(0, path.find_last_of('/'));

	// ������ڵ�
	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// ����Mesh
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	// �����ӽڵ�
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;

		// ����λ��
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		// ������
		if (mesh->HasNormals())
		{
			vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}

		// UV
		if (mesh->mTextureCoords[0])
		{
			vertex.texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else
		{
			vertex.texCoord = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		// Face��ΪͼԪPrimitive
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// ���ز��ʺ�����
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType aiTexType, TextureType texType)
{
	std::vector<Texture> texturesOutput;

	for (unsigned int i = 0; i < mat->GetTextureCount(aiTexType); ++i)
	{
		// ��ȡ��������
		aiString str;
		mat->GetTexture(aiTexType, i, &str);
		std::string path = directory + '/' + std::string(str.C_Str());

		// �ж��Ƿ��Ѽ��أ������ظ�����
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); ++j)
		{
			if (std::strcmp(textures_loaded[j].path.data(), path.data()) == 0)
			{
				skip = true;
				texturesOutput.push_back(textures_loaded[j]);
				break;
			}
		}

		// ��������
		if (!skip)
		{
			Texture texture(path.data(), texType);
			texturesOutput.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}

	return texturesOutput;
}

