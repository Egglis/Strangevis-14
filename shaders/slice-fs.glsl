#version 450

in vec3 tex;
out vec4 fragmentColor;

layout(location = 0) uniform sampler3D volumeTexture;
layout(location = 1) uniform sampler1D transferFunction;

void main(void)
{
    float volumeValue = texture(volumeTexture, tex).r;
    vec3 color = texture(transferFunction, volumeValue).rgb;
    fragmentColor = vec4(color, 1.0);
}
