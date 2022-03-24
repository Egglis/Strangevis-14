#version 450

in vec3 tex;
out vec4 fragmentColor;

uniform sampler3D volumeTexture;

void main(void)
{
    float volumeValue = texture(volumeTexture, tex).r;
    fragmentColor = vec4(pow(vec3(volumeValue),vec3(1/2.2)), 1.0);
}
