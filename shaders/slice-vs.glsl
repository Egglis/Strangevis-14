#version 450

in vec3 intersectionCoords;
out vec3 texCoords;

uniform mat4 modelViewMatrix;

void main(void)
{
    gl_Position =
        modelViewMatrix * vec4(intersectionCoords, 1.0f);
    texCoords = 0.5 * (intersectionCoords + 1.0f);
}
