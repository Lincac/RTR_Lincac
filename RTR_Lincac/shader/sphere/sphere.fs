#version 460 core

out vec4 FragColor;

const float PI = 3.14159265359;

in VS_OUT{
    vec3 fragpos;
    vec3 normal;
    vec2 TexCoords;
}fs_in;

struct Light{
    vec3 position;
    vec3 color;
    float radius;
};

uniform sampler2D Albedo;
uniform sampler2D Normal;
uniform sampler2D Metallic;
uniform sampler2D Roughness;
uniform sampler2D Ao;

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D LUTMap;

uniform sampler2DArray shadowMap;

uniform mat4 fs_view;
uniform vec3 viewPos;
uniform Light light;

uniform float far_plane;
uniform int countLayer;
uniform float planeDistances[16];

layout (std140, binding = 0) uniform LightSpaceMatrices  // std140 一种布局规则 
{
    mat4 lightSpaceMatrices[16];
};

float RadicalInverse_VdC(uint bits) // (0,1) 低差异序列
{
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
float get_average_field(vec3 currentPos,vec2 texelSize,int layer){
    int step = 3;
    float over_depth = 0.0;
    float sum_depth = 0.0;
    for(int x = -step ; x < step ;++x){
        for(int y = -step ; y < step ;++y){
            float sampleDepth = texture(shadowMap, vec3(currentPos.xy + vec2(x,y) * texelSize,layer)).r;
            if(sampleDepth < currentPos.z){
                over_depth += sampleDepth;
                sum_depth += 1;
            }
        }
    }
    if(over_depth > 0.0) return over_depth / sum_depth;
    return 0.0;
}
float ShadowCalculation(vec3 fragPosWorldSpace){
    vec4 fragPosViewSpace = fs_view * vec4(fragPosWorldSpace,1.0);
    float depthValue = abs(fragPosViewSpace.z);

    int layer = -1;
    for(int i = 0; i < countLayer ; i++){
        if(depthValue < planeDistances[i]){
            layer = i;
            break;
        }
    }
    if(layer == -1) layer = countLayer;

    vec4 fragPosLightSpace = lightSpaceMatrices[layer] * vec4(fragPosWorldSpace,1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float current = projCoords.z;
    if(current > 1.0) return 0.0;

    float bias = max(0.05 * (1.0 - dot(normalize(fs_in.normal),normalize(light.position))),0.005);
    if(layer == countLayer) bias *= 1.0 / (far_plane * 5); 
    else bias *= 1.0 / (planeDistances[layer] * 5); // 缩小偏移量

    // pcss
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap,0));

    float avg_depth = get_average_field(projCoords,texelSize,layer);  // 滤核小于目前点的平均深度
    if(avg_depth == 0.0) return 0.0;
    float d_blocker = avg_depth;
    float d_receiver = current;
    float filt = (d_receiver - d_blocker) * light.radius * 2 / d_blocker;
    if(filt == 0) return 0.0;

    // pcf
    filt = filt < 1.0 ? 2.0 : filt;
    float shadow = 0.0;
    int cout = 0;
    for(uint x = 0; x <= 4; x++)
    {
        cout++;
        for(uint y = 0; y <= 4; y++)
        {
            float Hx = RadicalInverse_VdC(x);
            float Hy = RadicalInverse_VdC(y);
            if(x / 2 == 0) Hx *= -1;
            if(y / 2 == 0) Hy *= -1;
            float pcfDepth = texture(shadowMap, vec3(projCoords.xy + vec2(Hx * filt,Hy * filt) * texelSize, layer)).r;
            shadow += (current - bias) > pcfDepth ? 1.0 : 0.0;      
        }    
    }

    shadow /= cout * cout;
    if(projCoords.z>1.0) shadow = 0.0;
    
    return shadow; 
}

vec3 fresnelSchlick(float cosTheta,vec3 F0){  // 计算不同观测角度的反射率
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   

float DistributionGGX(vec3 N,vec3 H,float roughness){  // 计算微表面中有多少和H方向相同
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N,H),0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float Ndot,float roughness){  // 计算几何遮蔽率
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = Ndot;
    float denom = Ndot * (1.0 - k) + k;

    return nom / denom;
}
float GeometrySmith(vec3 V,vec3 N,vec3 L, float roughness){ //观察方向(几何遮蔽)和光线方向(几何阴影)
    float NdotV = max(dot(N,V),0.0);
    float NdotL = max(dot(N,L),0.0);

    float ggx1 = GeometrySchlickGGX(NdotV,roughness);
    float ggx2 = GeometrySchlickGGX(NdotL,roughness);
    
    return ggx1 * ggx2;
}

vec3 getNormalFromMap(){
    vec3 tangentNormal = texture(Normal, fs_in.TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(fs_in.fragpos);
    vec3 Q2  = dFdy(fs_in.fragpos);
    vec2 st1 = dFdx(fs_in.TexCoords);
    vec2 st2 = dFdy(fs_in.TexCoords);

    vec3 N   = normalize(fs_in.normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main(){
    vec3 albedo = pow(texture(Albedo,fs_in.TexCoords).rgb,vec3(2.2));
    float metallic = texture(Metallic,fs_in.TexCoords).r;
    float roughness = texture(Roughness,fs_in.TexCoords).r;
    float ao = texture(Ao,fs_in.TexCoords).r;

    vec3 N = getNormalFromMap();
    vec3 V = normalize(viewPos - fs_in.fragpos);
    vec3 L = normalize(light.position - fs_in.fragpos); // 直接光
    vec3 H = normalize(V + L);
    vec3 R = reflect(-V,N);
    
    float distance = length(light.position - fs_in.fragpos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = light.color * attenuation;

    float shadow = ShadowCalculation(fs_in.fragpos);

    vec3 F0 = vec3(0.04); // 基础反射率(垂直入射)
    F0 = mix(F0,albedo,metallic);  // 线性混合 f0*(1-Metallic) + Albedo * Metallic

    vec3 L0 = vec3(0.0);
    // 直接光 
    vec3 F = fresnelSchlick(max(dot(N,V),0.0),F0);  
    float NDF = DistributionGGX(N,H,roughness);
    float G = GeometrySmith(V,N,L,roughness);

    // 直接光 漫反射部分
    vec3 KS = F;
    vec3 KD = vec3(1.0) - KS;
    KD *= 1.0 - metallic; //  折射率
    vec3 diffuse = KD * albedo / PI;

    // 直接光 镜面反射部分
    vec3 nominator = NDF * G * F;
    float denominator = 4.0 * max(dot(N,V),0.0) * max(dot(N,L),0.0) + 0.0001;
    vec3 specular = nominator / denominator;

    float NdotL = max(dot(N,L),0.0);
    L0 += (diffuse + specular) * radiance * NdotL * (1.0 - shadow);

    // 间接光
    vec3 f = fresnelSchlickRoughness(max(dot(N,V),0.0),F0,roughness);
    vec3 irradiance = texture(irradianceMap,N).rgb; // 环境辐照度
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap,R,roughness * MAX_REFLECTION_LOD).rgb; 
    vec2 brdf = texture(LUTMap,vec2(max(dot(N,V),0.0),roughness)).rg;

    // 间接光 漫反射部分  不受视线影响
    vec3 ks = f;
    vec3 kd = vec3(1.0) - ks;
    kd *= 1.0 - metallic; // 一般视为金属程度越高折射率越低
    diffuse = kd * irradiance * albedo;

    // 间接光 镜面反射部分 
    specular = prefilteredColor * (f * brdf.x + brdf.y);

    vec3 ambient = (diffuse + specular) * ao;

    // 最终结果
    L0 += ambient;

    FragColor = vec4(L0,1.0);
}