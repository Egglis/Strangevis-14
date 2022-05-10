#version 450

uniform mat4 modelViewProjectionMatrix;
uniform vec2 viewportSize;

in vec3 position;
flat out vec2 center;
flat out float pointSize;

void main(void) {
    const float size = 13.0;
    vec4 pos = modelViewProjectionMatrix * vec4(position, 1.0);

    center.xy = ((pos.xy / pos.w) + vec2(1.0)) * (viewportSize * 0.5);
    pointSize = size;

    gl_Position = pos;
    gl_PointSize = size;
}