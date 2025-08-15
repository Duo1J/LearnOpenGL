#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

// 物体颜色
uniform vec3 objectColor;
// 光源颜色
uniform vec3 lightColor;
// 光源位置
uniform vec3 lightPos;
// 相机位置
uniform vec3 viewPos;

// 环境光强度
float ambientStrength = 0.1;
//镜面反射强度
float specularStrength = 0.7;

void main()
{
    // 环境光
    vec3 ambient = ambientStrength * lightColor;

    // 漫反射
    // 归一化法向量
    vec3 normal = normalize(Normal);
    // 计算片段到光源的方向向量
    vec3 lightDir = normalize(lightPos - FragPos);
    // 计算最后的漫反射结果
    // dot(normal, lightDir) = |normal| * |lightDir| * cosΘ = cosΘ
    vec3 diffuse = max(dot(normal, lightDir), 0.0) * lightColor;

    // 镜面反射
    // 观察方向
    vec3 viewDir = normalize(viewPos - FragPos);

    // Phong光照模型
    // 光线反射向量
    //vec3 reflectDir = reflect(-lightDir, normal);
    //vec3 specular = pow(max(dot(viewDir, reflectDir), 0), 32) * specularStrength * lightColor;

    // Blinn-Phong光照模型
    vec3 halfDir = normalize(lightDir + viewDir);
    vec3 specular = pow(max(dot(halfDir, normal), 0), 32) * specularStrength * lightColor;

    FragColor = vec4(objectColor * (ambient + diffuse + specular), 1.0f);
}