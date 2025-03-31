#version 460

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec2 inUV;

layout (location = 1) out vec4 Color;
layout (location = 2) out vec2 UV;
layout(location = 3) out int instanceIndex;
layout(location = 4) out int textureIndex;

layout(push_constant) uniform constants {
    uint instanceIndex;
} PushConstants;

layout (set = 0, binding = 1) uniform UBOData {
    mat4 perspective;
    mat4 view;
} UBOdata;

#include "shader_parts/common/instance_data.glsl"

void main()
{
    // Feed fragment shader with vertex data
    UV = inUV;
    Color = inColor;
    instanceIndex = gl_InstanceIndex;// Pass the instance index
    textureIndex = instanceBuffer.objects[gl_InstanceIndex].textureIndex;

    // Model matrix
    mat4 modelMatrix = instanceBuffer.objects[gl_InstanceIndex].model;


    // Calculate MVP matrix
    gl_Position = UBOdata.perspective * UBOdata.view * modelMatrix * vec4(inPos, 1.0);
}
