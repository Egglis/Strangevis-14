#version 450
uniform vec3 selectedPoint;
uniform mat4 modelMatrix;
uniform float planeCorrection;

in vec3 vertex;
out vec4 fragmentColor;

void main(void)
{
    vec3 h = vec3(modelMatrix*vec4(selectedPoint,1.0f));
    vec3 j = vec3(modelMatrix*vec4(vertex,1.0f));
    vec3 diff = h-j;
    fragmentColor = length(diff) < 0.05f /planeCorrection ? vec4(1.0f, 0.0f, 0.0f, 0.3f) : vec4(0.3f);
}
