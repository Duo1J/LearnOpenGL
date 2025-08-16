#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // 聚光灯的方向
    vec3 direction;
    // 内切光角的cos值
    float cutOff;
    // 外切光角的cos值
    float cutOffOuter;

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
    vec3 lightDir = normalize(light.position - FragPos);
    float theta_cos = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.cutOffOuter;
    // 强度，直接作用于漫反射和镜面反射，由于会衰减到0，所以不需要再使用if-else来判断了
    float intensity = clamp((theta_cos - light.cutOffOuter) / epsilon, 0.0f, 1.0f);

    // 衰减系数
    float distance = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    // 环境光
    vec3 ambient = lightColor * vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    // 漫反射
    vec3 normal = normalize(Normal);
    vec3 diffuse = max(dot(normal, lightDir), 0.0) * lightColor * vec3(texture(material.diffuse, TexCoords)) * light.diffuse * intensity * attenuation;

    // 镜面反射
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specular = pow(max(dot(viewDir, reflectDir), 0), material.shininess) * lightColor * vec3(texture(material.specular, TexCoords)) * light.specular * intensity * attenuation;

    FragColor = vec4(ambient + diffuse + specular, 1.0f);
}