#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

// 平行光
struct DirLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 direction;
};

// 点光源
struct PointLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;

    float constant;
    float linear;
    float quadratic;
};

// 聚光灯
struct SpotLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
    vec3 direction;
    float cutOff;
    float cutOffOuter;

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

uniform DirLight dirLight;
#define POINT_LIGHT_NUM 4
uniform PointLight pointLight[POINT_LIGHT_NUM];
uniform SpotLight spotLight;

// 计算平行光
vec3 CalcDirectionalLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // 环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    // 漫反射
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    return ambient + diffuse + specular;
}

// 计算点光源
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // 衰减系数
    float d = length(light.position - fragPos);
    float attenuation = 1 / (light.constant + light.linear * d + light.quadratic * d * d);

    // 环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)) * attenuation;
    // 漫反射
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)) * attenuation;
    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords)) * attenuation;

    return ambient + diffuse + specular;
}

// 计算聚光灯
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // 强度
    float theta_cos = dot(lightDir, -normalize(light.direction));
    float intensity = clamp((theta_cos - light.cutOffOuter) / (light.cutOff - light.cutOffOuter), 0, 1);

    // 衰减
    float d = length(light.position - FragPos);
    float attenuation = 1 / (light.constant + light.linear * d + light.quadratic * d * d);

    // 环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)) * attenuation * intensity;
    // 漫反射
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)) * attenuation * intensity;
    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords)) * attenuation * intensity;

    return ambient + diffuse + specular;
}

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // 平行光
    vec3 result = CalcDirectionalLight(dirLight, normal, viewDir);
    // 点光源
    for (int i = 0; i < POINT_LIGHT_NUM; i++)
    {
        result += CalcPointLight(pointLight[i], normal, viewDir, FragPos);
    }

    // 聚光灯
    result += CalcSpotLight(spotLight, normal, viewDir, FragPos);

    FragColor = vec4(result, 1.0f);
}