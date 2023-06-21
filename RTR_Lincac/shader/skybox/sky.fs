#version 460 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube cubemap;

void main(){
    vec3 color = textureLod(cubemap,TexCoords,0.0).rgb; 
    color = color / (color + vec3(1.0));
    const float gamma = 2.2;
    color = pow(color,vec3(1.0 / gamma));
    FragColor = vec4(color,1.0);
}