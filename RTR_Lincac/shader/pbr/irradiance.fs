#version 450 core

out vec4 FragColor;

in vec3 locaPos;

uniform samplerCube cubeMap;

const float PI = 3.14159265357;

void main(){
    vec3 up = vec3(0.0,1.0,0.0);
    vec3 n = normalize(locaPos);
    vec3 t = normalize(cross(up,n));
    up = normalize(cross(n,t));
    mat3 tbn = mat3(t,up,n);

    vec3 irradiance = vec3(0.0);

    float sampleDelta = 0.025;
    int sampleNum = 0;
    for(float phi = 0.0 ; phi < 2.0 * PI ; phi += sampleDelta){
        for(float theta = 0.0 ;theta < 0.5 * PI ; theta += sampleDelta){
            vec3 temp = vec3(cos(phi) * sin(theta),sin(theta) * sin(phi), cos(theta));
            temp = tbn * temp;

            irradiance += texture(cubeMap,temp).rgb * cos(theta) * sin(theta);
            sampleNum++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(sampleNum));

    FragColor = vec4(irradiance,1.0);
}

// 这里的 temp = vec3(cos(phi) * sin(theta),sin(theta) * sin(phi), cos(theta))
// 因为是球形当theta变大时x变化并不是等距离的，所以乘以sin(theta)当作权重
// 同样的当phi变大时y变化也不是等距离的，所以乘以sin(phi)当作权重