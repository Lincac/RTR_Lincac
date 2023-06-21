#version 450 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D Tex;
uniform sampler2D Tex2;
// uniform sampler2DArray Tex;

uniform mat4 view;

float near = 0.1; 
float far  = 500.0; 
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}


void main(){
    float t = texture(Tex,TexCoords).r;
    // FragColor = texture(Tex,TexCoords) + texture(Tex2,TexCoords);
    //  FragColor = vec4(vec3(t),1.0);
    // vec3 pos = texture(Tex2,TexCoords).xyz;
    // pos = pos.x == 0.0 ? vec3(0) : vec3(view * vec4(pos,1.0));
    // FragColor = vec4(pos,1.0); 
    FragColor = texture(Tex2,TexCoords) + texture(Tex,TexCoords);
}