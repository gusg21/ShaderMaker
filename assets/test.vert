#version 330

layout(location = 0) in vec3 in_vPos; // Position in model space
layout(location = 1) in vec3 in_vNormal; // Normal in model space
layout(location = 2) in vec2 in_vUv;
layout(location = 3) in vec3 in_vTangent;
layout(location = 4) in vec3 in_vBitangent;

out Surface {
    vec3 vPosition; // Vertex position in world space
    vec3 vNormal; // Vertex normal in world space
    vec2 vUv;
    mat3 matTBN;
} out_sOut;

uniform mat4 u_matModel; // Model transform
uniform mat4 u_matView; // View transform

void main() {
    out_sOut.vPosition = vec3(u_matModel * vec4(in_vPos,1.0));
    out_sOut.vNormal = transpose(inverse(mat3(u_matModel))) * in_vNormal;
    out_sOut.vUv = in_vUv;

    vec3 T = normalize(vec3(u_matModel * vec4(in_vTangent,   0.0)));
    vec3 B = normalize(vec3(u_matModel * vec4(in_vBitangent, 0.0)));
    vec3 N = normalize(vec3(u_matModel * vec4(in_vNormal,    0.0)));
    out_sOut.matTBN = transpose(mat3(T, B, N));

    gl_Position = u_matView * u_matModel * vec4(in_vPos, 1.0);
}