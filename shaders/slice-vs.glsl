#version 450

in vec2 vertexPosition;
in vec3 texCoord;
out vec3 tex;

uniform mat4 modelViewMatrix;

void main(void)
{
    gl_Position = modelViewMatrix * vec4(vertexPosition, 0.0f, 1.0f);
    tex = 0.5 * (texCoord + 1.0f);
}
