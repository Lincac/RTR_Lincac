#version 460 core
layout (points) in;
layout (line_strip,max_vertices = 84) out;

uniform mat4 MVP;

void main(){ 
    // 水平线
    vec4 start = gl_in[0].gl_Position;
    vec4 end = gl_in[0].gl_Position + vec4(10.0,0.0,0.0,0.0);
    for(int i=1;i<=20;i++){
        gl_Position = MVP * start;
        EmitVertex();
        gl_Position = MVP * end;
        EmitVertex();
        EndPrimitive();    
        start.z += 0.5;
        end.z += 0.5;
    }
    gl_Position = MVP * start;
    EmitVertex();
    gl_Position = MVP * end;
    EmitVertex();
    EndPrimitive();    

    // 竖直线
    start = gl_in[0].gl_Position;
    end = gl_in[0].gl_Position + vec4(0.0,0.0,10.0,0.0);
    for(int i=1;i<=20;i++){
        gl_Position = MVP * start;
        EmitVertex();
        gl_Position = MVP * end;
        EmitVertex();
        EndPrimitive();    
        start.x += 0.5;
        end.x += 0.5;
    }
    gl_Position = MVP * start;
    EmitVertex();
    gl_Position = MVP * end;
    EmitVertex();
    EndPrimitive();    
}