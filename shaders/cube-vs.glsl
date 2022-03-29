#version 450

uniform vec4 clippingPlaneEquation;
uniform mat4 modelViewProjectionMatrix;
uniform mat4 gridSpacingMatrix;

in vec3 vertexPosition;
out vec3 volumePosition;

void main(void)
{
    gl_Position = modelViewProjectionMatrix*gridSpacingMatrix*vec4(vertexPosition,1.0f);
    volumePosition = 0.5*(vertexPosition+vec3(1.0));
    gl_ClipDistance[0] = dot(-clippingPlaneEquation.xyz, vertexPosition)+clippingPlaneEquation.w;
}
