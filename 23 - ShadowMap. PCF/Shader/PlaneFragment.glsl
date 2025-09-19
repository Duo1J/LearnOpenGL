#version 330 core

struct DirLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
// 光源空间的VP变换矩阵
in vec4 FragPosLightSpace;

uniform vec3 color;
uniform vec3 lightDir;
uniform vec3 viewPos;

// ShadowMap
uniform sampler2D shadowMap;

uniform DirLight dirLight;

vec3 CalcDirectionalLight(DirLight light, vec3 normal, vec3 viewDir, float shadow)
{
    vec3 lightDirection = normalize(lightDir);
    vec3 ambient = light.ambient * color;
    float diff = max(dot(normal, lightDirection), 0);
    vec3 diffuse = light.diffuse * diff * color;
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0), 32);
    vec3 specular = light.specular * spec * color;

    // 漫反射和镜面反射乘以shadow值
    return ambient + diffuse * shadow + specular * shadow;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // 执行透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    if (projCoords.z > 1.0)
        return 0;

    // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;

    float shadow = 0.0;
    // 获取纹素大小
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += projCoords.z > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    return shadow;
}

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    float shadow = ShadowCalculation(FragPosLightSpace);
	FragColor = vec4(CalcDirectionalLight(dirLight, normal, viewDir, (1 - shadow)), 1.0);
}