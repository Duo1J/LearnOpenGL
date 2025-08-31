#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
	vec3 I = normalize(Position - cameraPos);
	// 计算视线反射后的向量R
	vec3 R = reflect(I, normalize(Normal));
	// 采样skybox
	FragColor = vec4(texture(skybox, R).rgb, 1.0);
}