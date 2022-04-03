#version 450

in vec3 volumePosition;
out vec4 fragmentColor;

layout(location = 0) uniform sampler3D volumeTexture;
layout(location = 1) uniform sampler1D transferFunction;
uniform int gradientMethod;
uniform int width;
uniform int height;
uniform int depth;

vec3 calculateGradient();

void main(void)
{
    vec3 gradient = calculateGradient();
    float volumeValue = texture(volumeTexture, vec3(volumePosition.xyz)).r;
    vec4 color = texture(transferFunction, volumeValue);
    fragmentColor = color;
}

vec3 calculateGradient()
{
    vec3 gradient = vec3(0.0f);
    if (width == 0 || height == 0 || depth == 0)
    {
        return gradient;
    }

    float x = volumePosition.x;
    float y = volumePosition.y;
    float z = volumePosition.z;
    float dx = 1.0f / width;
    float dy = 1.0f / height;
    float dz = 1.0f / depth;

    if (gradientMethod == 0) // Central Difference
    {
        gradient.x = 1.0f/(2*dx) * (texture(volumeTexture, vec3(x + dx, y, z)).r -
                             texture(volumeTexture, vec3(x - dx, y, z)).r);
        gradient.y = 1.0f/(2*dy) * (texture(volumeTexture, vec3(x, y + dy, z)).r -
                             texture(volumeTexture, vec3(x, y - dy, z)).r);
        gradient.z = 1.0f/(2*dz) * (texture(volumeTexture, vec3(x, y, z + dz)).r -
                             texture(volumeTexture, vec3(x, y, z - dz)).r);
    }
    if (gradientMethod == 1) // Forward Difference
    {
        gradient.x = 1/dx*(texture(volumeTexture, vec3(x + dx, y, z)).r -
                     texture(volumeTexture, vec3(x, y, z)).r);
        gradient.y = 1/dy*(texture(volumeTexture, vec3(x, y + dy, z)).r -
                     texture(volumeTexture, vec3(x, y, z)).r);
        gradient.z = 1/dz*(texture(volumeTexture, vec3(x, y, z + dz)).r -
                     texture(volumeTexture, vec3(x, y, z)).r);
    }
    if (gradientMethod == 2) // Backward Difference
    {
        gradient.x = 1/dx*(texture(volumeTexture, vec3(x, y, z)).r -
                     texture(volumeTexture, vec3(x - dx, y, z)).r);
        gradient.y = 1/dy*(texture(volumeTexture, vec3(x, y, z)).r -
                     texture(volumeTexture, vec3(x, y - dy, z)).r);
        gradient.z = 1/dz*(texture(volumeTexture, vec3(x, y, z)).r -
                     texture(volumeTexture, vec3(x, y, z - dz)).r);
    }
    return gradient;
}
