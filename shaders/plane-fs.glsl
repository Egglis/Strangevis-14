#version 450

out vec4 fragmentColor;

uniform bool hideSlice;


void main(void)
{
    fragmentColor = (hideSlice) ? vec4(0) : vec4(0.3,0.3,0.3,0.3);
}
