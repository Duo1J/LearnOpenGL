#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 offset; // 实例化数组方式

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
// 去掉了多余的Position

uniform mat4 model;
layout (std140) uniform Matrices
{
    mat4 view;
    mat4 projection;
};

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Debug;
} vs_out;

void main()
{
    gl_Position = projection * view * model * vec4(aPos + offset, 1.0);
    FragPos = vec3(model * vec4(aPos + offset, 1.0));
    vs_out.FragPos = FragPos;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    vs_out.Normal = Normal;
    TexCoords = aTexCoords;
    vs_out.TexCoords = aTexCoords;
}