#version 330 core

struct DirLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

uniform vec3 color;
uniform vec3 lightDir;
uniform vec3 viewPos;

// 点光源远平面
uniform float farPlane;

uniform sampler2D shadowMap;
// 点光源立方体贴图
uniform samplerCube pointShadowMap;

uniform DirLight dirLight;
// 点光源光照数据
uniform PointLight pointLight;

vec3 CalcDirectionalLight(DirLight light, vec3 normal, vec3 viewDir, float shadow)
{
    vec3 lightDirection = normalize(lightDir);
    vec3 ambient = light.ambient * color;
    float diff = max(dot(normal, lightDirection), 0);
    vec3 diffuse = light.diffuse * diff * color;
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0), 32);
    vec3 specular = light.specular * spec * color;

    return ambient + diffuse * shadow + specular * shadow;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos, float shadow)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float d = length(light.position - fragPos);
    float attenuation = 1 / (light.constant + light.linear * d + light.quadratic * d * d);

    vec3 ambient = light.ambient * color * attenuation;
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = light.diffuse * diff * color * attenuation;
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0), 32);
    vec3 specular = light.specular * spec * color * attenuation;

    return ambient + diffuse * shadow + specular * shadow;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    if (projCoords.z > 1.0)
        return 0;

    projCoords = projCoords * 0.5 + 0.5;

    float shadow = 0.0;
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

float PointShadowCalculation(vec3 fragPos, PointLight light)
{
	// 方向
    vec3 fragToLight = fragPos - light.position;
    // 当前片段深度
    float currentDepth = length(fragToLight);
    // 深度超出远平面，无阴影
    if (currentDepth > farPlane)
        return 0;

	// 采样深度
    float closestDepth = texture(pointShadowMap, fragToLight).r;
   	// 深度值乘以远平面距离得到深度
    closestDepth *= farPlane;
    float bias = 0.05;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    return shadow;
}

float PointShadowCalculationPCF(vec3 fragPos, PointLight light)
{
	float shadow = 0.0;
    vec3 fragToLight = fragPos - light.position;
    float currentDepth = length(fragToLight);
    if (currentDepth > farPlane)
        return 0;

    float samples = 4;
    float offset = 0.1;
    for (float x = -offset; x < offset; x += offset / (samples * 0.5))
    {
        for (float y = -offset; y < offset; y += offset / (samples * 0.5))
        {
            for (float z = -offset; z < offset; z += offset / (samples * 0.5))
            {
                float closestDepth = texture(pointShadowMap, fragToLight + vec3(x, y, z)).r; 
                closestDepth *= farPlane;
                if (currentDepth > closestDepth)
                    shadow += 1.0;
            }
        }
    }
    shadow /= (samples * samples * samples);

    return shadow;
}

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    float dirShadow = ShadowCalculation(FragPosLightSpace);
    float pointShadow = PointShadowCalculation(FragPos, pointLight);

    vec3 result = CalcDirectionalLight(dirLight, normal, viewDir, (1 - pointShadow));
    result += CalcPointLight(pointLight, normal, viewDir, FragPos, (1 - pointShadow));

    FragColor = vec4(result, 1.0);

//    vec3 fragToLight = FragPos - pointLight.position;
//    float closestDepth = texture(pointShadowMap, fragToLight).r;
//    FragColor = vec4(vec3(closestDepth / farPlane), 1.0);
}