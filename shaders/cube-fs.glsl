#version 450

in vec3 volumePosition;
out vec4 fragmentColor;

layout(location = 0) uniform sampler3D volumeTexture;
layout(location = 1) uniform sampler1D transferFunction;
uniform int gradientMethod;
uniform int width;
uniform int height;
uniform int depth;

struct Ray
{
    vec3 origin;
    vec3 direction;
    vec3 invDirection;
} ray;

struct Box
{
    vec3 minCorner; // -1,-1,-1
    vec3 maxCorner; //  1, 1, 1
} box;

vec3 calculateGradient();
Ray generateRay(vec3 origin, vec3 direction);
bool rayCubeIntersection(Ray ray, Box box, out float tmin, out float tmax);

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
        gradient.x = 1.0f / (2 * dx) *
                     (texture(volumeTexture, vec3(x + dx, y, z)).r -
                      texture(volumeTexture, vec3(x - dx, y, z)).r);
        gradient.y = 1.0f / (2 * dy) *
                     (texture(volumeTexture, vec3(x, y + dy, z)).r -
                      texture(volumeTexture, vec3(x, y - dy, z)).r);
        gradient.z = 1.0f / (2 * dz) *
                     (texture(volumeTexture, vec3(x, y, z + dz)).r -
                      texture(volumeTexture, vec3(x, y, z - dz)).r);
    }
    if (gradientMethod == 1) // Forward Difference
    {
        gradient.x = 1 / dx *
                     (texture(volumeTexture, vec3(x + dx, y, z)).r -
                      texture(volumeTexture, vec3(x, y, z)).r);
        gradient.y = 1 / dy *
                     (texture(volumeTexture, vec3(x, y + dy, z)).r -
                      texture(volumeTexture, vec3(x, y, z)).r);
        gradient.z = 1 / dz *
                     (texture(volumeTexture, vec3(x, y, z + dz)).r -
                      texture(volumeTexture, vec3(x, y, z)).r);
    }
    if (gradientMethod == 2) // Backward Difference
    {
        gradient.x = 1 / dx *
                     (texture(volumeTexture, vec3(x, y, z)).r -
                      texture(volumeTexture, vec3(x - dx, y, z)).r);
        gradient.y = 1 / dy *
                     (texture(volumeTexture, vec3(x, y, z)).r -
                      texture(volumeTexture, vec3(x, y - dy, z)).r);
        gradient.z = 1 / dz *
                     (texture(volumeTexture, vec3(x, y, z)).r -
                      texture(volumeTexture, vec3(x, y, z - dz)).r);
    }
    return gradient;
}

// Pre-calculate the inverse direction so intersection tests don't recalculate
// this
Ray generateRay(vec3 origin, vec3 direction)
{
    Ray ray;
    ray.origin = origin;
    ray.direction = direction;
    ray.invDirection = 1.0f/direction;
    return ray;
}

// Slab method for intersection
// We know the axis-aligned bounding box is cornered at -1..1
// If it returns true, it intersected the box entering at tmin and exiting at
// tmax
bool rayCubeIntersection(Ray ray, Box box, out float tmin, out float tmax)
{
    float tx0 = ray.invDirection.x*(box.minCorner.x - ray.origin.x);
    float tx1 = ray.invDirection.x*(box.maxCorner.x - ray.origin.x);

    tmin = min(tx0, tx1);
    tmax = max(tx0, tx1);

    float ty0 = ray.invDirection.y*(box.minCorner.y - ray.origin.y);
    float ty1 = ray.invDirection.y*(box.maxCorner.y - ray.origin.y);

    tmin = max(tmin, min(ty0, ty1));
    tmax = min(tmax, max(ty0, ty1));

    float tz0 = ray.invDirection.z*(box.minCorner.z - ray.origin.z);
    float tz1 = ray.invDirection.z*(box.maxCorner.z - ray.origin.z);

    tmin = max(tmin, min(tz0, ty1));
    tmax = min(tmax, max(tz0, tz1));

    return tmin <= tmax;
}
