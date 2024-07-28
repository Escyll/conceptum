#version 450 core

in VS_OUT {
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
uniform vec3 viewPos;

out vec4 fragColor;

void main()
{
    // vec4 alphaDiffuseMapColor = useDiffuseTexture ? texture(diffuseTexture, fs_in.uv * diffuseScale.xy) : vec4(1.0);
    // vec3 diffuseMapColor = alphaDiffuseMapColor.rgb;
    // vec3 diffuseMapColor = vec3(1.0, 0.0, 0.0);
    vec3 uNormal = normalize(fs_in.normal);
    // if (useNormalMap)
    // {
    //     uNormal = texture(normalMap, fs_in.uv * normalScale.xy).xyz;
    //     uNormal = uNormal * 2.0 - 1.0;
    //     uNormal.xy *= normalStrength;
    //     uNormal = normalize(fs_in.tbn * uNormal);
    // }

    // Diffuse
    vec3 lightDir = normalize(lightPos - fs_in.worldPos);
    float diff = max(dot(uNormal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 result = (vec3(0.2, 0.2, 0.2) + 0.8*diffuse) * diffuseColor;
    
    // Ambient
    // float ambientStrength = 0.3;
    // vec3 ambient = ambientStrength * lightColor * diffuseMapColor;
    
    // Specular
    // float specularStrength = 0.2;
    // vec3 viewDir = normalize(viewPos - fs_in.worldPos);
    // vec3 reflectDir = reflect(-lightDir, uNormal);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularExponent);
    // vec3 specular = specularStrength * spec * lightColor * diffuseMapColor;

    // fragColor = vec4((ambient + diffuse + specular), alphaDiffuseMapColor.a);
    fragColor = vec4(result, 1.0);
}
