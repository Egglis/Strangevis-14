#version 450

uniform bool headLight;

out vec4 fragmentColor;


void main(void) {

    fragmentColor = (headLight) ? vec4(0,0,0,0) : vec4(0.960, 0.564, 0.258, 0.7);
    
}