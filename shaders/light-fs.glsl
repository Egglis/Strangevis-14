#version 450

uniform bool headLight;

flat in vec2 center;
flat in float pointSize;
out vec4 fragmentColor;


void main(void) {

    float centerDistance = sqrt(4.0)*length(gl_FragCoord.xy-center)/pointSize;

    if(headLight) {
        fragmentColor = vec4(0,0,0,0);
        return;
    }

    fragmentColor = vec4(0.960, 0.564, 0.258, 0.7);
}