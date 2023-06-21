#version 460 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D Albedo;
uniform sampler2D Normal;

uniform vec3 LightCol;
uniform vec3 LightDir;
uniform vec3 viewPos;
uniform vec3 _Color;
uniform vec3 _Specular;

uniform float Gloss;
uniform bool use_albedoMap;
uniform bool use_normalMap;
uniform vec3 Ambient;

vec3 getNormalFromMap()
{
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

vec3 colorTolum(vec3 c){
    return c / (dot(c,vec3(0.3,0.6,0.1)));
}

void main()
{           
    vec3 normal = use_normalMap ? getNormalFromMap() : normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(LightDir + viewDir);  

    vec3 albedo = pow(texture(Albedo,fs_in.TexCoords).rgb,vec3(2.2));
    albedo = use_albedoMap ? albedo * _Color : _Color;
    // ambient
    vec3 ambient = Ambient * albedo;
    // diffuse
    vec3 diffuse = LightCol * albedo * max(dot(LightDir, normal), 0.0);
    // specular
    vec3 specular = LightCol * _Specular * pow(max(dot(halfwayDir,normal), 0.0), Gloss);    
    vec3 color = ambient + diffuse + specular;
    
    vec3 mapped = color / (color + vec3(1.0));   // hdr to ldr
    mapped = pow(mapped,vec3(1.0 / 2.2));

    FragColor = vec4(mapped, 1.0);
}