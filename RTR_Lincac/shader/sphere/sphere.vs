#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT{
    vec3 fragpos;
    vec3 normal;
    vec2 TexCoords;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * vec4(aPos,1.0);

    vs_out.TexCoords = aTexCoords;
    vs_out.normal = mat3(model) * aNormal;
    vs_out.fragpos = vec3(model * vec4(aPos,1.0));
}