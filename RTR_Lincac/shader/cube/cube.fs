#version 450 core

out vec4 FragColor;

in VS_OUT{
    vec3 fragpos;
    vec3 normal;
    vec2 TexCoords;
}fs_in;

struct Light{
    vec3 position;
    vec3 color;
};

uniform sampler2D Albedo;
uniform sampler2D Normal;
uniform sampler2DArray shadowMap;

uniform Light light;
uniform vec3 viewpos;
uniform float far_plane;

uniform int countLayer;
uniform float planeDistances[16];
uniform mat4 fs_view;

layout (std140, binding = 0) uniform LightSpaceMatrices 
{
    mat4 lightSpaceMatrices[16];
};

vec3 getNormalFromMap(){
    vec3 tangentNormal = texture(Normal, fs_in.TexCoords).xyz * 2.0 - 1.0;

    // 利用顶点上得uv坐标计算TBN
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

float ShadowCalculation(vec3 fragPosWorldSpace){
    vec4 fragPosViewSpace =  fs_view * vec4(fragPosWorldSpace,1.0);
    float depthValue = abs(fragPosViewSpace.z);  // 观测空间深度

    int layer = -1;
    for(int i = 0; i < countLayer ;i++){
        if(depthValue < planeDistances[i]){
            layer = i;   // 判断深度层次，获取对应的平截头体
            break;
        }
    }
    if(layer == -1) layer = countLayer;

    vec4 fragPosLightSpace = lightSpaceMatrices[layer] * vec4(fragPosWorldSpace,1.0);  // 坐标转化为对应的NDC
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float current = projCoords.z;
    if(current > 1.0) return 0.0;

    // 计算偏移量
    float bias = max(0.05 * (1.0 - dot(normalize(fs_in.normal),normalize(light.position))),0.005);
    if(layer == countLayer) bias *= 1.0 / (far_plane * 5);
    else bias *= 1.0 / (planeDistances[layer] * 5);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap,0));
    for(int i = -1;i<=2;i++){
        for(int j = -1; j<=2;j++){
            float pcfDepth = texture(shadowMap,vec3(projCoords.xy + vec2(i,j) * texelSize,layer)).r;
            shadow += (current - bias) > pcfDepth ? 1.0 :0.0;
        }
    }
    shadow /= 16.0;
    if(projCoords.z > 1.0) shadow = 0.0;
    return shadow;
}
void main(){
    vec3 color = texture(Albedo,fs_in.TexCoords).rgb;
    vec3 lightdir = normalize(light.position);
    vec3 viewdir = normalize(viewpos - fs_in.fragpos);
    vec3 norm = getNormalFromMap();

    vec3 ambtient = light.color * color * color;

    float diff = max(dot(lightdir,norm),0.0);
    vec3 diffuse = diff * light.color * color;

    vec3 halfway = normalize(viewdir + lightdir);
    float spec = pow(max(dot(halfway,norm),0.0),64.0);
    vec3 specular = light.color * spec * color;

    float shadow = ShadowCalculation(fs_in.fragpos);
    vec3 result = ambtient + (1.0 - shadow) * (diffuse + specular);

    FragColor = vec4(result,1.0);
}