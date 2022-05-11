#version 450

uniform mat4 modelViewProjectionMatrix;
uniform vec2 viewportSize;

in vec3 position;

void main(void) {
    const float size = 13.0;
    vec4 pos = modelViewProjectionMatrix * vec4(position, 1.0);

    gl_Position = pos;
    gl_PointSize = size;
}