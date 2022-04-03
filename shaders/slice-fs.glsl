#version 450

in vec3 texCoords;
out vec4 fragmentColor;

layout(location = 0) uniform sampler3D volumeTexture;
layout(location = 1) uniform sampler1D transferFunction;

void main(void)
{
    float volumeValue = texture(volumeTexture, texCoords).r;
    vec4 color = texture(transferFunction, volumeValue);
    fragmentColor = color;
}
