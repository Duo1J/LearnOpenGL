#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
// 接收纹理坐标
layout (location = 2) in vec2 aTexCoord;

out vec3 vertexColor;
out vec2 texCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    vertexColor = aColor;
   	// 将纹理坐标传给后续的片段着色器，以供采样
    texCoord = aTexCoord;
}