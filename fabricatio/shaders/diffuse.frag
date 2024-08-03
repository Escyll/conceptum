#version 450 core

in VS_OUT {
    vec3 worldPos;
    vec2 uv;
    mat3 tbn;
    vec3 normal;
} fs_in;  

uniform vec3 diffuseColor;
uniform sampler2D diffuseTexture;
uniform vec3 diffuseScale;

uniform vec3 lightPos;
uniform vec3 lightColor;

out vec4 fragColor;

void main()
{
    vec4 alphaDiffuseMapColor = texture(diffuseTexture, fs_in.uv * diffuseScale.xy);
    vec3 diffuseMapColor = alphaDiffuseMapColor.rgb;
    vec3 uNormal = normalize(fs_in.normal);

    // Diffuse
    vec3 lightDir = normalize(lightPos - fs_in.worldPos);
    float diff = max(dot(uNormal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 result = (vec3(0.2, 0.2, 0.2) + 0.8*diffuse) * diffuseMapColor;
    
    // Ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor * diffuseMapColor;

    fragColor = vec4((ambient + diffuse), alphaDiffuseMapColor.a);
}
