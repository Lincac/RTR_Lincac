#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D HDR;

void main(){
    vec3 hdrcolor = texture(HDR,TexCoords).rgb;

    vec3 mapped = hdrcolor / (hdrcolor + vec3(1.0));   // hdr to ldr

    mapped = pow(mapped,vec3(1.0 / 2.2));

    FragColor = vec4(mapped,1.0); 
}