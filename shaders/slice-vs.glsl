#version 450

uniform mat4 modelViewMatrix;
in vec4 vertexPosition;
out vec2 slicePosition;
uniform int slicingPlane;

void main(void)
{
    gl_Position = vertexPosition;
    slicePosition = 0.5 * ((modelViewMatrix*vertexPosition).xy + 1.0f);
}
