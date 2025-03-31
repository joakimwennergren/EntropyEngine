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

struct InstanceData{
    mat4 model;
    vec4 bgColor;
    vec4 borderColor;
    vec4 cornerRadiuses;
    vec2 dimension;
    int type;
    int textureIndex;
};

//all object matrices
layout(std140, set = 0, binding = 0) readonly buffer InstanceBuffer{
    InstanceData objects[];
} instanceBuffer;

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
