#version 450

in vec3 volumePosition;
out vec4 fragmentColor;

uniform sampler3D volumeTexture;

void main(void)
{
    float volumeValue = texture(volumeTexture, vec3(volumePosition.xyz)).r;
    fragmentColor = vec4(volumeValue, volumeValue, volumeValue,1.0);
}