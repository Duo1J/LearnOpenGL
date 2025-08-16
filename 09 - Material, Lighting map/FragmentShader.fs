#version 330 core

// ������ʽṹ��
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

// �����ܹ���ǿ�Ƚṹ��
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // ������
    vec3 ambient = lightColor * vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    // ������
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 diffuse = max(dot(normal, lightDir), 0.0) * lightColor * vec3(texture(material.diffuse, TexCoords)) * light.diffuse;

    // ���淴��
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specular = pow(max(dot(viewDir, reflectDir), 0), material.shininess) * lightColor * vec3(texture(material.specular, TexCoords)) * light.specular;

    FragColor = vec4(ambient + diffuse + specular, 1.0f);
}