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

    // �۹�Ƶķ���
    vec3 direction;
    // ���й�ǵ�cosֵ
    float cutOff;
    // ���й�ǵ�cosֵ
    float cutOffOuter;

    // ˥������
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
    // ǿ�ȣ�ֱ��������������;��淴�䣬���ڻ�˥����0�����Բ���Ҫ��ʹ��if-else���ж���
    float intensity = clamp((theta_cos - light.cutOffOuter) / epsilon, 0.0f, 1.0f);

    // ˥��ϵ��
    float distance = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    // ������
    vec3 ambient = lightColor * vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    // ������
    vec3 normal = normalize(Normal);
    vec3 diffuse = max(dot(normal, lightDir), 0.0) * lightColor * vec3(texture(material.diffuse, TexCoords)) * light.diffuse * intensity * attenuation;

    // ���淴��
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specular = pow(max(dot(viewDir, reflectDir), 0), material.shininess) * lightColor * vec3(texture(material.specular, TexCoords)) * light.specular * intensity * attenuation;

    FragColor = vec4(ambient + diffuse + specular, 1.0f);
}