#version 450

#include "shader_parts/common/vertex_layout.glsl"
#include "shader_parts/common/push_constants.glsl"
#include "shader_parts/common/instance_data.glsl"

layout (set = 0, binding = 1) uniform UboInstance
{
    mat4 perspective;
    mat4 view;

} uboInstance;

void main()
{
    // Feed fragment shader with vertex data
    Normal = inNormal;
    UV = inUV;
    Color = inColor;
    
    // Model matrix
    mat4 modelMatrix = instanceBuffer.objects[PushConstants.instanceIndex].model;

    // Calculate MVP matrix
    gl_Position = uboInstance.perspective * uboInstance.view * modelMatrix * vec4(inPos, 1.0);
}
