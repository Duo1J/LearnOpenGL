#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

// ������ɫ
uniform vec3 objectColor;
// ��Դ��ɫ
uniform vec3 lightColor;
// ��Դλ��
uniform vec3 lightPos;
// ���λ��
uniform vec3 viewPos;

// ������ǿ��
float ambientStrength = 0.1;
//���淴��ǿ��
float specularStrength = 0.7;

void main()
{
    // ������
    vec3 ambient = ambientStrength * lightColor;

    // ������
    // ��һ��������
    vec3 normal = normalize(Normal);
    // ����Ƭ�ε���Դ�ķ�������
    vec3 lightDir = normalize(lightPos - FragPos);
    // ����������������
    // dot(normal, lightDir) = |normal| * |lightDir| * cos�� = cos��
    vec3 diffuse = max(dot(normal, lightDir), 0.0) * lightColor;

    // ���淴��
    // �۲췽��
    vec3 viewDir = normalize(viewPos - FragPos);

    // Phong����ģ��
    // ���߷�������
    //vec3 reflectDir = reflect(-lightDir, normal);
    //vec3 specular = pow(max(dot(viewDir, reflectDir), 0), 32) * specularStrength * lightColor;

    // Blinn-Phong����ģ��
    vec3 halfDir = normalize(lightDir + viewDir);
    vec3 specular = pow(max(dot(halfDir, normal), 0), 32) * specularStrength * lightColor;

    FragColor = vec4(objectColor * (ambient + diffuse + specular), 1.0f);
}