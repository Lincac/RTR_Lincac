#version 460 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

const float PI = 3.14159265359;

uniform sampler2D Albedo;
uniform sampler2D Normal;
uniform sampler2D Specular;
uniform sampler2D Metallic;
uniform sampler2D Roughness;
uniform sampler2D Ao;

uniform vec3 viewPos;

uniform vec3 _Color;
uniform vec3 _Specular;
uniform vec3 LightDir;
uniform vec3 LightCol;
uniform float _Roughness;
uniform float _Metallic;
uniform float _Ao;

uniform bool use_albedoMap;
uniform bool use_normalMap;
uniform bool use_specularMap; 
uniform bool use_roughnessMap;
uniform bool use_metallicMap;
uniform bool use_aoMap;
uniform bool use_skybox;

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D LUTMap;

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

    vec3 Q1  = dFdx(fs_in.FragPos);
    vec3 Q2  = dFdy(fs_in.FragPos);
    vec2 st1 = dFdx(fs_in.TexCoords);
    vec2 st2 = dFdy(fs_in.TexCoords);

    vec3 N   = normalize(fs_in.Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main(){
    vec3 albedo = use_albedoMap ? pow(texture(Albedo,fs_in.TexCoords).rgb,vec3(2.2)) : _Color;
    vec3 normal = use_normalMap ? getNormalFromMap() : fs_in.Normal;
    float metallic = use_metallicMap ? texture(Metallic,fs_in.TexCoords).r : _Metallic;
    float roughness = use_roughnessMap ? texture(Roughness,fs_in.TexCoords).r : _Roughness;
    float ao = use_aoMap ? texture(Ao,fs_in.TexCoords).r : _Ao;
    vec3 _specularCol = use_specularMap ? texture(Specular,fs_in.TexCoords).rgb : _Specular;

    vec3 N = normal;
    vec3 V = normalize(viewPos - fs_in.FragPos);
    vec3 L = normalize(LightDir); // 直接光
    vec3 H = normalize(V + L);
    vec3 R = reflect(-V,N);
    
    vec3 F0 = vec3(0.04); // 基础反射率(垂直入射)
    F0 = mix(F0,albedo,metallic);  // 线性混合 f0*(1-Metallic) + Albedo * Metallic

    vec3 L0 = vec3(0.0);
    // 直接光 
    vec3 F = fresnelSchlick(max(dot(N,V),0.0),F0);  
    float NDF = DistributionGGX(N,H,roughness);
    float G = GeometrySmith(V,N,L,roughness);

    vec3 KS = F;
    vec3 KD = vec3(1.0) - KS;
    KD *= 1.0 - metallic; //  折射率
    vec3 diffuse = KD * albedo / PI;

    vec3 nominator = NDF * G * F;
    float denominator = 4.0 * max(dot(N,V),0.0) * max(dot(N,L),0.0) + 0.0001;
    vec3 specular = _specularCol * nominator / denominator ;

    float NdotL = max(dot(N,L),0.0);
    L0 += (diffuse + specular) * LightCol * NdotL * ao;
    if(!use_skybox){
        vec3 mapped = L0 / (L0 + vec3(1.0));   // hdr to ldr
        mapped = pow(mapped,vec3(1.0 / 2.2));
        FragColor = vec4(mapped,1.0);
        return;
    }

    // 间接光
    vec3 f = fresnelSchlickRoughness(max(dot(N,V),0.0),F0,roughness);
    vec3 irradiance = texture(irradianceMap,N).rgb; // 环境辐照度
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap,R,roughness * MAX_REFLECTION_LOD).rgb; 
    vec2 brdf = texture(LUTMap,vec2(max(dot(N,V),0.0),roughness)).rg;

    vec3 ks = f;
    vec3 kd = vec3(1.0) - ks;
    kd *= 1.0 - metallic; // 一般视为金属程度越高折射率越低
    diffuse = kd * irradiance * albedo;

    specular = prefilteredColor * (f * brdf.x + brdf.y);

    vec3 ambient = (diffuse + specular) * ao;

    L0 += ambient;

    vec3 mapped = L0 / (L0 + vec3(1.0));   // hdr to ldr
    mapped = pow(mapped,vec3(1.0 / 2.2));
    FragColor = vec4(mapped,1.0);
}