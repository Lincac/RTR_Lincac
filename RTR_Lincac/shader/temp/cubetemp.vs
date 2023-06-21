#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 localpos;

void main(){
    mat4 v = mat4(mat3(view));
    vec4 pos = projection * v * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
    localpos = vec3(model * vec4(aPos,1.0));
}