#version 450 core

out vec4 FragColor;

in vec3 localpos;

uniform samplerCube Tex;

void main(){
    FragColor = textureLod(Tex,localpos,1.2);
}