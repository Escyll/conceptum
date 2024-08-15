#version 450 core

layout (location = 0) in vec3 modelPos;
layout (location = 1) in vec2 modelUV;
layout (location = 2) in vec3 modelNormal;
layout (location = 3) in vec3 modelTangent;
layout (location = 4) in vec3 modelBitangent;

// layout(location=5) out VS_OUT {
//     vec3 worldPos;
//     vec2 uv;
//     mat3 tbn;
//     vec3 normal;
// } vs_out;

// uniform float time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // gl_Position = projection * view * model * vec4(modelPos, 1.0f);
    gl_Position = projection * view * model * vec4(modelPos, 1.0f);
    // vs_out.worldPos = vec3(model * vec4(modelPos, 1.0f));
    // vs_out.uv = modelUV;
    // // vs_out.normal = transpose(inverse(mat3(model))) * modelNormal; // Move to CPU
    // vs_out.normal = modelNormal;
    // vec3 t = normalize(vec3(model * vec4(modelTangent, 0.0)));
    // vec3 b = normalize(vec3(model * vec4(modelBitangent, 0.0)));
    // vec3 n = normalize(vec3(model * vec4(modelNormal, 0.0)));
    // vs_out.tbn = mat3(t, b, n);
}
