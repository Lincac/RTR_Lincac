#version 450 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D Tex;
uniform vec3 lightCol;

void main(){
    vec4 color = texture(Tex,TexCoords);
    if(color.r > 0.1){
        discard;
    }
    FragColor = color + vec4(lightCol,1);
}