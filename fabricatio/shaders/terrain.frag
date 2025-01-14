#version 450 core

layout(location=0) in VS_OUT {
    vec3 worldPos;
    vec2 uv;
    mat3 tbn;
    vec3 normal;
} fs_in;

uniform vec3 diffuseColor;
uniform bool useDiffuseTexture;
uniform sampler2D diffuseTexture;
uniform vec3 diffuseScale;

uniform bool useNormalMap;
uniform sampler2D normalMap;
uniform float normalStrength;
uniform vec3 normalScale;

uniform vec3 specularColor;
uniform float specularExponent;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform sampler2D grass;
uniform sampler2D rock;

layout(location=0) out vec4 myOutputColor;

void main()
{
    float distance = length(lightPos - fs_in.worldPos);
    float lightConstant = 1.0;
    float lightLinear = 0.022;
    float lightQuadratic = 0.0019;
    float attenuation = 1.0 / (lightConstant + lightLinear * distance + lightQuadratic * (distance * distance));
    vec3 uNormal = normalize(fs_in.normal);

    vec3 lightDir = normalize(lightPos - fs_in.worldPos);
    float diff = max(dot(uNormal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * attenuation;
    if (dot(uNormal, vec3(0,0,1)) > 0.5 && fs_in.worldPos.z > 30.0)
        myOutputColor = texture(grass, fs_in.worldPos.xy*2) * vec4(diffuse+0.1, 1.0);
    else if (dot(uNormal, vec3(0,0,1)) > 0.0 && fs_in.worldPos.z > 30.0)
        myOutputColor = 0.5*(texture(grass, fs_in.worldPos.xy*2) + texture(rock, fs_in.worldPos.xz*2)) * vec4(diffuse+0.1, 1.0);
    else
        myOutputColor = texture(rock, fs_in.worldPos.xz*0.5) * vec4(diffuse+0.1, 1.0);
}
