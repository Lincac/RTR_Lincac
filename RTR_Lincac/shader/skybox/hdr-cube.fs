#version 450 core

out vec4 FragColor;

in vec3 locaPos;

uniform sampler2D hdrskybox;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 getuv(vec3 v){
    vec2 uv = vec2(atan(v.z,v.x),asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;    
}

void main(){
    vec2 uv  = getuv(normalize(locaPos));
    vec3 color = pow(texture(hdrskybox,uv).rgb,vec3(2.2));

    FragColor = vec4(color,1.0);
}