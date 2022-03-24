#version 450

in vec3 tex;
out vec4 fragmentColor;

layout(location = 0) uniform sampler3D volumeTexture;
layout(location = 1) uniform sampler1D transferTexture;

void main(void)
{
    float volumeValue = texture(volumeTexture, tex).r;
    vec4 tfn = texture(transferTexture, volumeValue);
    fragmentColor = vec4(vec3(volumeValue), 1.0);
    fragmentColor = vec4(tfn.rgb, 1.0);
}
