#version 330
// Uniforms + Inputs
out vec4 out_vColor;
in vec2 vUv;

uniform sampler2D _ColorBuffer;

// Utility Functions
float add(float a, float b) {
    return a + b;
}

vec3 add(vec3 a, vec3 b) {
    return a + b;
}

float subtract(float a, float b) {
    return a - b;
}

vec3 subtract(vec3 a, vec3 b) {
    return a - b;
}

vec3 subtract(vec3 a, float b) {
    return a - b;
}

float multiply(float a, float b) {
    return a * b;
}

vec3 multiply(vec3 a, vec3 b) {
    return a * b;
}

vec3 multiply(vec3 a, float b) {
    return a * b;
}

vec3 multiply(mat3 a, vec3 b) {
    return a * b;
}

float invert(float a) {
    return -a;
}

vec3 invert(vec3 a) {
    return -a;
}

vec3 get_rgb(vec4 vec) {
    return vec.rgb;
}




void main() {
    // CODE GOES HERE
    vec3 color = texture(_ColorBuffer, vUv).rgb;
    out_vColor = vec4(color, 1.0);
}