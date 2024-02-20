#version 450 core

in vec3 vertexPos;
in vec2 TexCoord;

uniform bool drawTexture;
uniform sampler2D diffuseTexture;

out vec4 FragColor;

float near = 0.1; 
float far  = 14.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
    float depth = LinearizeDepth(gl_FragCoord.z)/far;
    // FragColor = drawTexture ? texture(ourTexture, TexCoord) : vec4(vertexColor, 1.0);
    // FragColor.r = clamp(FragColor.r, 0, 1 - 2*depth);
    // FragColor.g = clamp(FragColor.g, 0, min(1, 1 - 2*depth + 0.9));
    // FragColor.b = clamp(FragColor.b, 0, min(1, 1 - 2*depth + 1.0));
    // float depth = LinearizeDepth(gl_FragCoord.z) / far; 
    // FragColor = vec4(vec3(depth), 1.0);

    // FragColor = vec4(vec3((vertexPos.x+0.9)/1.8,(vertexPos.y+0.9)/1.8,vertexPos.z-2.5f - 3.0f), 1.0f);
    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    FragColor = texture(diffuseTexture, TexCoord);
}
