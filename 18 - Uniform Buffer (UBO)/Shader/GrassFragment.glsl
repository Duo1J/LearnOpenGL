#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D tex_diffuse1;

void main()
{
	vec4 diffuse = texture(tex_diffuse1, TexCoords);

	// 丢弃掉alpha值小于0.1的片段
	if (diffuse.a < 0.1)
	{
		discard;
	}

	FragColor = diffuse;
}