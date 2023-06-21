#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 camPos;
uniform vec3 lightPos;

void main(){
    vec3 center = vec3(model * vec4(lightPos,1.0));

    vec3 worldPos = vec3(model * vec4(aPos,1.0));
    vec3 viewDir = camPos - worldPos;
    vec3 normalDir = normalize(viewDir);

    vec3 upDir = abs(normalDir.y) > 0.999 ? vec3(0, 0, 1) : vec3(0, 1, 0);
    vec3 rightDir = normalize(cross(upDir,normalDir));
    upDir = normalize(cross(normalDir,rightDir));

    vec3 centerOffs = worldPos - center;
    vec3 newPos = center + rightDir * centerOffs.x + upDir * centerOffs.y + normalDir * centerOffs.z;

    gl_Position = projection * view * vec4(newPos,1.0);
    TexCoords = aTexCoords;
}