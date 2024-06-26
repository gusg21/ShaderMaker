#version 330

// Uniforms + Inputs
struct Material {
    float nAmbient; //Ambient coefficient (0-1)
    float nDiffuse; //Diffuse coefficient (0-1)
    float nSpecular; //Specular coefficient (0-1)
    float nShininess; //Affects size of specular highlight
};

in Surface {
    vec3 vPosition; // Vertex position in world space
    vec3 vNormal; // Vertex normal in world space
    vec2 vUv;
    mat3 matTBN;
} in_sIn;

uniform sampler2D u_texMain;
uniform sampler2D u_texNormal;

uniform vec3 u_vEyePos;

uniform vec3 u_vLightDirection = vec3(-1.0, -1.0, 0.0);
uniform vec3 u_vLightColor = vec3(1.0);
uniform vec3 u_vAmbientColor = vec3(0.3, 0.4, 0.46);

uniform Material u_sMaterial;

out vec4 out_vColor;

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
}
 