#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D tex_diffuse1;

void main()
{
	vec4 diffuse = texture(tex_diffuse1, TexCoords);

	// ������alphaֵС��0.1��Ƭ��
	if (diffuse.a < 0.1)
	{
		discard;
	}

	FragColor = diffuse;
}