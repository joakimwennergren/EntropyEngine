#version 460

// Vertex attributes
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inUV;

// Instanced attributes
layout (location = 2) in vec3 instancePos;
layout (location = 3) in vec3 instanceRot;
layout (location = 4) in vec3 instanceScale;
layout (location = 5) in int instanceTexIndex;

layout (location = 2) out vec2 outUV;

layout (binding = 1) uniform UBO {
    mat4 projection;
    mat4 modelview;
} ubo;

void main()
{
    // Feed fragment shader with vertex data
    outUV = inUV;

    vec4 locPos = vec4(inPos.xyz, 1.0);
    vec4 pos = vec4((locPos.xyz) + instancePos, 1.0);

    // Calculate MVP matrix
    gl_Position = ubo.projection * ubo.modelview * pos;
}
