#version 460 core

out vec4 FragColor;

in vec3 locaPos;

uniform samplerCube cubeMap;
uniform float roughness;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float RadicalInverse_VdC(uint bits) 
{
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
// ----------------------------------------------------------------------------
vec2 Hammersley(uint i, uint N)
{
	return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}

vec3 ImportanceSampleGGX(vec2 xi,vec3 N,float roughness){
    float a = roughness * roughness;

    float phi = 2.0 * PI * xi.x; 
    float cosTheta = sqrt((1.0 - xi.y) / (1.0 + (a*a - 1.0) * xi.y));
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta); 

    // 单位球内的向量
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sinTheta * sin(phi);
    H.z = cosTheta;

	vec3 up          = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);  // 当采样向量是正对中心时，需要改变up方向，因为向量自己叉乘为零向量
	vec3 tangent   = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);

    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}

void main(){
    vec3 N = normalize(locaPos);
    vec3 R = N;
    vec3 V = R;

    const uint SAMPLE_COUNT = 1024u;
    float totalWeight = 0.0;
    vec3 prefilterColor = vec3(0.0);
    for(uint i = 0u; i<SAMPLE_COUNT;i++){
        vec2 xi = Hammersley(i,SAMPLE_COUNT);
        vec3 H = ImportanceSampleGGX(xi,N,roughness);
        vec3 L = normalize(2.0 * dot(V,H) * H - V); // 三角形法则

        float NdotL = max(dot(N,L),0.0);
        if(NdotL > 0.0){
            float D   = DistributionGGX(N, H, roughness);
            float NdotH = max(dot(N, H), 0.0);
            float HdotV = max(dot(H, V), 0.0);
            float pdf = D * NdotH / (4.0 * HdotV) + 0.0001; 

            float resolution = 512.0; // resolution of source cubemap (per face)
            float saTexel  = 4.0 * PI / (6.0 * resolution * resolution); // 球的面积平均到立方体图上
            float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

            float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel); 

            prefilterColor += textureLod(cubeMap, L, mipLevel).rgb * NdotL;
            totalWeight += NdotL;
        }
    }
    prefilterColor = prefilterColor / totalWeight;

    FragColor = vec4(prefilterColor,1.0);

}
