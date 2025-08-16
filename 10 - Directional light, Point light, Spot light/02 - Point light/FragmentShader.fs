#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    // 点光源位置
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // 衰减参数
    float constant;
    float linear;
    float quadratic;
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
    // 与光源的距离
    float distance = length(light.position - FragPos);
    // 强度
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    // 环境光
    vec3 ambient = lightColor * vec3(texture(material.diffuse, TexCoords)) * light.ambient * attenuation;

    // 漫反射
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 diffuse = max(dot(normal, lightDir), 0.0) * lightColor * vec3(texture(material.diffuse, TexCoords)) * light.diffuse * attenuation;

    // 镜面反射
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specular = pow(max(dot(viewDir, reflectDir), 0), material.shininess) * lightColor * vec3(texture(material.specular, TexCoords)) * light.specular * attenuation;

    FragColor = vec4(ambient + diffuse + specular, 1.0f);
}