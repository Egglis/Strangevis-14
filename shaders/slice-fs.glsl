#version 450

in vec3 tex;
out vec4 fragmentColor;

uniform sampler3D volumeTexture;

void main(void)
{
    float volumeValue = texture(volumeTexture, tex).r;
    fragmentColor = vec4(vec3(volumeValue), 1.0);
}
