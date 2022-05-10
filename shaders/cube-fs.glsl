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

uniform vec3 lpos;

// Render Settings:
uniform float ambientInt;
uniform float diffuseInt;
uniform float specInt;
uniform float specCoeff;
uniform bool specOff;
uniform bool maxInt;

uniform int stepSize;

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

struct Sphere {
	vec3 pos;
	float r;
} s;

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

vec2 raySphereIntersection(Ray ray, const Sphere s) {
    vec3 l = s.pos - ray.origin;
    float tca = dot(l, ray.direction);
    float d2 = dot(l, l) - tca * tca;
    if (d2 > s.r * s.r)
        return vec2(-1.0);
    float thc = sqrt(s.r * s.r - d2);
    float t0 = tca - thc;
    float t1 = tca + thc;
    if (t0 < t1 && t1 >= 0.0)
        return vec2(t0, t1);
    return vec2(-1.0);
}

vec3 ShadeBlinnPhong (vec3 pos, vec3 ld, vec3 vd, vec3 clr)
{
    vec3 normal = calculateGradient(pos);
    if(normal != vec3(0,0,0)){

        normal = normalize(normal);
        vec3 lightDir = normalize(ld);
        vec3 eyeDir = normalize(vd);

        float dotDiff = max(0, dot(lightDir, normal));

        float specularValue = 0.0;
        if(dotDiff > 0.0 && specOff){
            vec3 H = normalize(lightDir + eyeDir);
            float specAngle = max(0, dot(normal, H));
            specularValue = pow(specAngle, specCoeff);
        }

        
        vec3 specular = vec3(1,1,1)*specInt*specularValue;

        clr = clr * (ambientInt + (diffuseInt * dotDiff));
        clr = clr + specular;
    }

  return clr;
}

// From INF251 project, converts a ray position into OpenGL z-value 
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

    stepLength = 1.0f/float(textureSize(volumeTexture, 0).x);

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
    
    /*
    Sphere s = Sphere(lpos, 2.0f);
    vec2 bounds = raySphereIntersection(castingRay, s);
    if(bounds.x > 0.0 && bounds.x < bounds.y && bounds.x < 1000000 && bounds.x < 100000) {
        fragmentColor = vec4(1,0,0,1);
        return;
    }
    */
    
    vec3 rayStart = (rayOrigin + rayDirection * tmin - bottom) / (top - bottom);
    vec3 rayEnd = (rayOrigin + rayDirection * tmax - bottom) / (top - bottom);

    vec3 ray = rayEnd - rayStart;
    float rayLength = length(ray);
    vec3 stepVector = stepLength * ray / rayLength;

    rayStart += stepVector;

    vec3 position = rayStart;
    float maxIntensity = 0.0f;

    vec4 color = vec4(0.0);

    while (rayLength > 0)
    {
        float intensity = texture(volumeTexture, position).r;

        if(maxInt) {
            maxIntensity = max(intensity, maxIntensity);
        } else {
            vec3 gradient = calculateGradient(position);
            vec4 src = texture(transferFunction, intensity);
            vec3 viewDir = rayOrigin - position;

            if(src.a > 0.0){          
                
                src.rgb = ShadeBlinnPhong(position, -lpos, viewDir, src.rgb);

                src.a = 1.0 - exp(-src.a * rayLength);
                src.rgb = src.rgb * src.a;
                color = color + (1.0 - color.a) * src;
                
                if(color.a > 0.99) {
                    gl_FragDepth = calcDepth(position);
                    break;
                }
            }
        }

        rayLength -= stepLength;
        position += stepVector;
        gl_FragDepth = calcDepth(position);
    }

    if(maxInt) {
        color = texture(transferFunction, maxIntensity);
    }

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


    gradient.x = 1.0f / (2 * dx) *
                (texture(volumeTexture, vec3(x + dx, y, z)).r -
                texture(volumeTexture, vec3(x - dx, y, z)).r);
    gradient.y = 1.0f / (2 * dy) *
                (texture(volumeTexture, vec3(x, y + dy, z)).r -
                texture(volumeTexture, vec3(x, y - dy, z)).r);
    gradient.z = 1.0f / (2 * dz) *
                (texture(volumeTexture, vec3(x, y, z + dz)).r -
                texture(volumeTexture, vec3(x, y, z - dz)).r);
    return gradient;
}
