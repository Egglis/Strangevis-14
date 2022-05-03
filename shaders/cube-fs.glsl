#version 450

out vec4 fragmentColor;

layout(location = 0) uniform sampler3D volumeTexture;
layout(location = 1) uniform sampler1D transferFunction;

uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec2 viewportSize;
uniform float aspectRatio;
uniform float focalLength;
uniform vec3 rayOrigin;

uniform int gradientMethod;
uniform int width;
uniform int height;
uniform int depth;

float stepLength = 0.01;

struct Ray
{
    vec3 origin;
    vec3 direction;
} ray;

struct AABB
{
    vec3 top;
    vec3 bottom;
} box;

vec3 calculateGradient(vec3 volumePosition);

// Slab-intersection method from https://martinopilia.com/posts/2018/09/17/volume-raycasting.html
void rayBoxIntersection(Ray ray, AABB box, out float tmin, out float tmax)
{
    vec3 invDirection = 1.0 / ray.direction;
    vec3 tTop = invDirection * (box.top - ray.origin);
    vec3 tBottom = invDirection * (box.bottom - ray.origin);
    vec3 tMin = min(tTop, tBottom);
    vec2 t = max(tMin.xx, tMin.yz);
    tmin = max(0.0, max(t.x, t.y));
    vec3 tMax = max(tTop, tBottom);
    t = min(tMax.xx, tMax.yz);
    tmax = min(t.x, t.y);
}

float calcDepth(vec3 pos)
{
	float far = gl_DepthRange.far; 
	float near = gl_DepthRange.near;
	vec4 clip_space_pos = viewMatrix * vec4(pos, 1.0);
	float ndc_depth = clip_space_pos.z / clip_space_pos.w;
	return (((far - near) * ndc_depth) + near + far) / 2.0;
}

void main(void)
{
    // Ray-direction calculated by method from https://martinopilia.com/posts/2018/09/17/volume-raycasting.html

    stepLength = 1.0f/1000;

    vec3 rayDirection;
    rayDirection.xy = 2.0 * gl_FragCoord.xy / viewportSize - 1.0;
    rayDirection.x *= aspectRatio;
    rayDirection.z = -focalLength;
    rayDirection = (vec4(rayDirection, 0) * viewMatrix).xyz;

    float tmin, tmax;
    Ray castingRay = Ray(rayOrigin, rayDirection);
    vec3 top = vec3(modelMatrix * vec4(1, 1, 1, 0));
    vec3 bottom = vec3(modelMatrix * vec4(-1, -1, -1, 0));
    AABB boundingBox = AABB(top, bottom);
    rayBoxIntersection(castingRay, boundingBox, tmin, tmax);

    vec3 rayStart = (rayOrigin + rayDirection * tmin - bottom) / (top - bottom);
    vec3 rayEnd = (rayOrigin + rayDirection * tmax - bottom) / (top - bottom);

    vec3 ray = rayEnd - rayStart;
    float rayLength = length(ray);
    vec3 stepVector = stepLength * ray / rayLength;

    // Perlin Noise
    rayStart += stepVector;//*(perlin_noise(rayEnd));

    vec3 position = rayStart;
    float maxIntensity = 0.0f;

    vec4 color = vec4(0,0,0,0);
    while (rayLength > 0 && color.a < 1.0)
    {
        
        float intensity = texture(volumeTexture, position).r;
        vec3 gradient = calculateGradient(position);
        vec4 c = texture(transferFunction, intensity);
        
        // Back to front color bledning
        color.rgb = (c.a * c.rgb + (1 - c.a) * color.a * color.rgb);
        color.a = c.a + (1 - c.a) * color.a;
        

        rayLength -= stepLength;
        position += stepVector;
    }

    gl_FragDepth = calcDepth(position);
    fragmentColor = color;
}


vec3 calculateGradient(vec3 volumePosition)
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
