#version 330 core

struct Material {
    sampler2D tex_diffuse1;
    sampler2D tex_diffuse2;
    // ...
    sampler2D tex_specular1;
    sampler2D tex_specular2;
    // ...
    float shininess;
};

struct DirLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 direction;
};

struct PointLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;

    float constant;
    float linear;
    float quadratic;
};

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

uniform vec3 viewPos;

uniform Material material;

uniform DirLight dirLight;
#define POINT_LIGHT_NUM 4
uniform PointLight pointLight[POINT_LIGHT_NUM];
uniform SpotLight spotLight;

uniform float near;
uniform float far;

// 计算平行光
vec3 CalcDirectionalLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // 环境光
    vec3 ambient = light.ambient * vec3(texture(material.tex_diffuse1, TexCoords));
    // 漫反射
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.tex_diffuse1, TexCoords));
    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.tex_specular1, TexCoords));

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
    vec3 ambient = light.ambient * vec3(texture(material.tex_diffuse1, TexCoords)) * attenuation;
    // 漫反射
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.tex_diffuse1, TexCoords)) * attenuation;
    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.tex_specular1, TexCoords)) * attenuation;

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
    vec3 ambient = light.ambient * vec3(texture(material.tex_diffuse1, TexCoords)) * attenuation * intensity;
    // 漫反射
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.tex_diffuse1, TexCoords)) * attenuation * intensity;
    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.tex_specular1, TexCoords)) * attenuation * intensity;

    return ambient + diffuse + specular;
}

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));    
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

    // 深度值
    float depth = (LinearizeDepth(gl_FragCoord.z) - near) / (far - near);
    FragColor = vec4(vec3(depth), 1.0);
}