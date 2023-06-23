#version 460 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D show_texture;
uniform bool show;

uniform vec3 viewPos;
uniform vec3 _Color;

void main()
{        
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);
    float diff = dot(viewDir,normal);
    vec3 color = diff * _Color;
    vec3 sampleColor = texture(show_texture,fs_in.TexCoords).rgb;
    color = show ? sampleColor * color : color;

    FragColor = vec4(color, 1.0);
}