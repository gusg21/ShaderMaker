#version 330

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

void main() {
    //Make sure fragment normal is still length 1 after interpolation.
    vec3 normal = texture(u_texNormal, in_sIn.vUv).rgb;
    normal = normal * 2.0 - 1.0;
    //Light pointing straight down
    vec3 toLight = in_sIn.matTBN * -normalize(u_vLightDirection);
    float diffuseFactor = max(dot(normal, toLight), 0.0);
    //Calculate specularly reflected light
    vec3 toEye = in_sIn.matTBN * normalize(u_vEyePos - in_sIn.vPosition);
    //Blinn-phong uses half angle
    vec3 h = normalize(toLight + toEye);
    float specularFactor = pow(max(dot(normal, h), 0.0), u_sMaterial.nShininess);
    //Combination of specular and diffuse reflection
    vec3 lightColor = (u_sMaterial.nDiffuse * diffuseFactor + u_sMaterial.nSpecular * specularFactor) * u_vLightColor;
    lightColor += u_vAmbientColor * u_sMaterial.nAmbient;
    vec3 objectColor = texture(u_texMain, in_sIn.vUv).rgb;

    out_vColor = vec4(objectColor * lightColor, 1.0);
}
