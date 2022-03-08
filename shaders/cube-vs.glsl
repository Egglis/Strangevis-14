#version 450

uniform vec4 clippingPlaneEquation;
uniform mat4 modelViewProjectionMatrix;

in vec4 vertexPosition;
out vec3 volumePosition;

void main(void)
{
    gl_Position = modelViewProjectionMatrix*vertexPosition;
    volumePosition = 0.5*(vertexPosition.xyz+vec3(1.0));
    gl_ClipDistance[0] = dot(clippingPlaneEquation.xyz, vertexPosition.xyz)+clippingPlaneEquation.w;
}
