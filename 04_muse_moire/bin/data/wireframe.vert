#version 120

// this doesn't work with es2

void main(){
    gl_FrontColor = gl_Color;
    gl_Position = ftransform();
}