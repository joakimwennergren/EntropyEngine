#version 460

// Vertex attributes
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inUV;

// Instanced attributes
layout (location = 2) in vec2  instancePos;
layout (location = 3) in vec2  instanceDim;
layout (location = 4) in float instanceRot;
layout (location = 5) in float instanceTexIndex;
layout (location = 6) in vec4  instanceAtlasCoords;
layout (location = 7) in vec4  instanceColor;

layout (location = 2) out vec3 outUV;
layout (location = 3) out vec4 outAtlasCoords;
layout (location = 4) out vec4 outColor;

layout (binding = 1) uniform UBO {
    mat4 projection;
    mat4 modelview;
} ubo;

void main() {
    outUV = vec3(inUV, instanceTexIndex);
    outAtlasCoords = instanceAtlasCoords;
    outColor = instanceColor;

    // Scale local position
    vec2 scaledPos = inPos.xy * instanceDim;

    // Apply 2D rotation
    float s = sin(instanceRot);
    float c = cos(instanceRot);
    vec2 rotatedPos = vec2(
        scaledPos.x * c - scaledPos.y * s,
        scaledPos.x * s + scaledPos.y * c
    );

    // Translate
    vec3 worldPos = vec3(rotatedPos + instancePos, 0.0);

    // Final position
    gl_Position = ubo.projection * ubo.modelview * vec4(worldPos, 1.0);
}
