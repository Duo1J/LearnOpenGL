#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 offset;
// 切线
layout (location = 4) in vec3 aTangent;
// 副切线
layout (location = 5) in vec3 aBitangent;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform vec3 viewPos;

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
    mat3 TBN;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
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

    // 计算TBN矩阵
    vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
    mat3 TBN = transpose(mat3(T, B, N));
    // 还是将TBN交给片段着色器用来计算lightPos，就先不移到顶点着色器了
    vs_out.TBN = TBN;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vec3(model * vec4(aPos + offset, 0.0));
}