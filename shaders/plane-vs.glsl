#version 450

uniform mat4 modelViewProjectionMatrix;

in vec3 vertexPosition;

void main(void)
{
    gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1.0f);
}
