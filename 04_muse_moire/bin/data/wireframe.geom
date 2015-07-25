// http://cgit.freedesktop.org/mesa/demos/commit/?id=7ba23583b237d4c07823006ef35dca434ebd5759

#version 120
//#extension GL_ARB_geometry_shader4: enable
//#extension GL_EXT_geometry_shader4 : enable
//#extension GL_EXT_gpu_shader4 : enable
uniform vec2 ViewportSize;
varying vec2 Vert0, Vert1, Vert2;

// Transform NDC coord to window coord
vec2 vpxform(vec4 p){
    return (p.xy / p.w + 1.0) * 0.5 * ViewportSize;
}

void main(){
    gl_FrontColor = gl_FrontColorIn[0];
    Vert0 = vpxform(gl_PositionIn[0]);
    Vert1 = vpxform(gl_PositionIn[1]);
    Vert2 = vpxform(gl_PositionIn[2]);
    gl_Position = gl_PositionIn[0];
    EmitVertex();
    gl_Position = gl_PositionIn[1];
    EmitVertex();
    gl_Position = gl_PositionIn[2];
    EmitVertex();
}