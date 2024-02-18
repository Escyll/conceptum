#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;

out vec3 vertexPos;
out vec2 TexCoord;

uniform float time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.0f);
    vertexPos = (view * model * vec4(pos, 1.0f)).xyz;
    TexCoord = texCoord;
}
