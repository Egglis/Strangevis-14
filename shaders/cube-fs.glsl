#version 450

in vec3 volumePosition;
out vec4 fragmentColor;

uniform sampler3D volumeTexture;
uniform int gradientMethod;
uniform int width;
uniform int height;
uniform int depth;

vec3 calculateGradient();

void main(void)
{
    vec3 gradient = calculateGradient();
    // float volumeValue = texture(volumeTexture, vec3(volumePosition.xyz)).r;
    // fragmentColor = vec4(volumeValue, volumeValue, volumeValue, 1.0);
    fragmentColor = vec4(gradient, 1.0);
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

    // Note the methods currently rotate the colors for emphasis!
    // This should not be used in a rotated way when raycasting.
    if (gradientMethod == 0) // Central Difference
    {
        // xyz
        gradient.x = 0.5f * (texture(volumeTexture, vec3(x + dx, y, z)).r -
                             texture(volumeTexture, vec3(x - dx, y, z)).r);
        gradient.y = 0.5f * (texture(volumeTexture, vec3(x, y + dy, z)).r -
                             texture(volumeTexture, vec3(x, y - dy, z)).r);
        gradient.z = 0.5f * (texture(volumeTexture, vec3(x, y, z + dz)).r -
                             texture(volumeTexture, vec3(x, y, z - dz)).r);
    }
    if (gradientMethod == 1) // Forward Difference
    {
        // yxz
        gradient.y = texture(volumeTexture, vec3(x + dx, y, z)).r -
                     texture(volumeTexture, vec3(x, y, z)).r;
        gradient.z = texture(volumeTexture, vec3(x, y + dy, z)).r -
                     texture(volumeTexture, vec3(x, y, z)).r;
        gradient.x = texture(volumeTexture, vec3(x, y, z + dz)).r -
                     texture(volumeTexture, vec3(x, y, z)).r;
    }
    if (gradientMethod == 2) // Backward Difference
    {
        // zxy
        gradient.z = texture(volumeTexture, vec3(x, y, z)).r -
                     texture(volumeTexture, vec3(x - dx, y, z)).r;
        gradient.x = texture(volumeTexture, vec3(x, y, z)).r -
                     texture(volumeTexture, vec3(x, y - dy, z)).r;
        gradient.y = texture(volumeTexture, vec3(x, y, z)).r -
                     texture(volumeTexture, vec3(x, y, z - dz)).r;
    }
    return gradient;
}
