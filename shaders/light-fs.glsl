#version 450


flat in vec2 center;
flat in float pointSize;
out vec4 fragmentColor;


void main(void) {

    float dist = sqrt(4.0)*length(gl_FragCoord.xy-center)/pointSize;
    fragmentColor = vec4(1, 0, 0, 1);
}