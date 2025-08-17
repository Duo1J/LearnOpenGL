#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

// ƽ�й�
struct DirLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 direction;
};

// ���Դ
struct PointLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;

    float constant;
    float linear;
    float quadratic;
};

// �۹��
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

// ����ƽ�й�
vec3 CalcDirectionalLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // ������
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    // ������
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    // ���淴��
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    return ambient + diffuse + specular;
}

// ������Դ
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // ˥��ϵ��
    float d = length(light.position - fragPos);
    float attenuation = 1 / (light.constant + light.linear * d + light.quadratic * d * d);

    // ������
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)) * attenuation;
    // ������
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)) * attenuation;
    // ���淴��
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords)) * attenuation;

    return ambient + diffuse + specular;
}

// ����۹��
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // ǿ��
    float theta_cos = dot(lightDir, -normalize(light.direction));
    float intensity = clamp((theta_cos - light.cutOffOuter) / (light.cutOff - light.cutOffOuter), 0, 1);

    // ˥��
    float d = length(light.position - FragPos);
    float attenuation = 1 / (light.constant + light.linear * d + light.quadratic * d * d);

    // ������
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)) * attenuation * intensity;
    // ������
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)) * attenuation * intensity;
    // ���淴��
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords)) * attenuation * intensity;

    return ambient + diffuse + specular;
}

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // ƽ�й�
    vec3 result = CalcDirectionalLight(dirLight, normal, viewDir);
    // ���Դ
    for (int i = 0; i < POINT_LIGHT_NUM; i++)
    {
        result += CalcPointLight(pointLight[i], normal, viewDir, FragPos);
    }

    // �۹��
    result += CalcSpotLight(spotLight, normal, viewDir, FragPos);

    FragColor = vec4(result, 1.0f);
}