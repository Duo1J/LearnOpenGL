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

// ����ƽ�й�
vec3 CalcDirectionalLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // ������
    vec3 ambient = light.ambient * vec3(texture(material.tex_diffuse1, TexCoords));
    // ������
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.tex_diffuse1, TexCoords));
    // ���淴��
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.tex_specular1, TexCoords));

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
    vec3 ambient = light.ambient * vec3(texture(material.tex_diffuse1, TexCoords)) * attenuation;
    // ������
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.tex_diffuse1, TexCoords)) * attenuation;
    // ���淴��
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.tex_specular1, TexCoords)) * attenuation;

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
    vec3 ambient = light.ambient * vec3(texture(material.tex_diffuse1, TexCoords)) * attenuation * intensity;
    // ������
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.tex_diffuse1, TexCoords)) * attenuation * intensity;
    // ���淴��
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

    // ƽ�й�
    vec3 result = CalcDirectionalLight(dirLight, normal, viewDir);
    // ���Դ
    for (int i = 0; i < POINT_LIGHT_NUM; i++)
    {
        result += CalcPointLight(pointLight[i], normal, viewDir, FragPos);
    }
    // �۹��
    result += CalcSpotLight(spotLight, normal, viewDir, FragPos);

    // ���ֵ
    float depth = (LinearizeDepth(gl_FragCoord.z) - near) / (far - near);
    FragColor = vec4(vec3(depth), 1.0);
}