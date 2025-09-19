#pragma once

#include <vector>
#include <string>

#include <glm.hpp>

// ��Ƭ����
float quadVertices[] = {
	// positions         // texture Coords
	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
	1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};

// ��λ��
std::vector<glm::vec3> grassPositions
{
	glm::vec3(1.5f, 0.0f, 0.51f),
	glm::vec3(0.0f, 0.0f, 0.7f),
};

// ����λ��
std::vector<glm::vec3> windowPositions
{
	glm::vec3(0.0f, 0.0f, 2.7f),
	glm::vec3(-0.2f, 0.0f, 1.71f),
	glm::vec3(0.2f, 0.5f, 0.51f),
};

// ��ĻQuad����
float screenQuadVertices[] = {
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
};

// ��պ�����
std::vector<std::string> skyboxFaces
{
	"F:/Scripts/Cpp/LearnOpenGL/learn-open-gl/Resource/skybox/T_Right.jpg",
	"F:/Scripts/Cpp/LearnOpenGL/learn-open-gl/Resource/skybox/T_Left.jpg",
	"F:/Scripts/Cpp/LearnOpenGL/learn-open-gl/Resource/skybox/T_Top.jpg",
	"F:/Scripts/Cpp/LearnOpenGL/learn-open-gl/Resource/skybox/T_Bottom.jpg",
	"F:/Scripts/Cpp/LearnOpenGL/learn-open-gl/Resource/skybox/T_Front.jpg",
	"F:/Scripts/Cpp/LearnOpenGL/learn-open-gl/Resource/skybox/T_Back.jpg"
};

// ��պ�Cube����
float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

// ���Դλ��
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

// ������ɫ��С���Ӷ���
float housePoints[] = {
	-0.5f,  0.5f,
};

float planeVertices[] = {
	// positions            // normals       
	 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,
	-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,
	 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,
	-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,
	-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,
	 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,
};

float cubeVertices[] = {
	// back face
	-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
	// front face
	-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	// left face
	-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
	-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
	-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
	-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
	-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
	-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
	// right face
	 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
	 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
	 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
	 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
	 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
	 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
	 // bottom face
	 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
	  1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
	  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
	  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
	 -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
	 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
	 // top face
	 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	  1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f,
	  1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
};