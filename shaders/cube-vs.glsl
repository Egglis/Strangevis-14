#version 450

uniform vec4 clippingPlaneEquation;
uniform mat4 modelViewProjectionMatrix;

in vec3 vertexPosition;

void main(void)
{
    gl_Position = modelViewProjectionMatrix*vec4(vertexPosition,1.0f);
    gl_ClipDistance[0] = dot(-clippingPlaneEquation.xyz, vertexPosition)+clippingPlaneEquation.w;
}
