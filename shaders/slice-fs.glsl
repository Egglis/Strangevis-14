#version 450

in vec2 slicePosition;
out vec4 fragmentColor;

uniform sampler3D volumeTexture;
uniform float slice;
uniform int slicingPlane;

void main(void)
{
    vec3 texCoord;
    if (slicingPlane == 0)
    {
        texCoord = vec3(slicePosition, slice);
    }
    if (slicingPlane == 1)
    {
        texCoord = vec3(slicePosition.x, slice, slicePosition.y);
    }
    if (slicingPlane == 2)
    {
        texCoord = vec3(slice, slicePosition.yx);
    }
    float volumeValue = texture(volumeTexture, texCoord).r;
    fragmentColor = vec4(pow(vec3(volumeValue), vec3(1.0f / 2.2f)), 1.0);
}
