#version 450

out vec4 fragmentColor;

uniform bool showSlice;


void main(void)
{
    fragmentColor = showSlice ? vec4(0.3) : vec4(0);
}
