#version 460

// Vertex attributes
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inUV;

// Instanced attributes
layout (location = 2) in vec2 instancePos;
layout (location = 3) in vec2 instanceDim;
layout (location = 4) in float instanceRot;
layout (location = 5) in int instanceTexIndex;
layout (location = 6) in vec4 instanceAtlasCoords;

layout (location = 2) out vec2 outUV;
layout (location = 3) out vec4 outAtlasCoords;

layout (binding = 1) uniform UBO {
    mat4 projection;
    mat4 modelview;
} ubo;

void main() {
    outUV = inUV;
    outAtlasCoords = instanceAtlasCoords;
    vec4 localPos = vec4(inPos * vec3(instanceDim, 0.0), 1.0);
    vec4 worldPos = vec4(localPos.xyz + vec3(instancePos, 0.0), 1.0);
    gl_Position = ubo.projection * ubo.modelview * worldPos;
}
