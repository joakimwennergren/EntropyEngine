#version 460
layout(set = 0, binding = 2) uniform sampler2D textures[100];
//layout(set = 0, binding = 2) uniform sampler2D Sampler2D;

layout (location = 1) in vec4 Color;
layout (location = 2) in vec2 UV;
layout(location = 3) flat in int instanceIndex;
layout(location = 4) flat in int textureIndex;

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform constants {
    uint instanceIndex;
} PushConstants;

layout (set = 0, binding = 1) uniform UBOData {
    mat4 perspective;
    mat4 view;
} UBOdata;

#include "shader_parts/common/instance_data.glsl"
#include "shader_parts/common/object_types.glsl"
#include "shader_parts/2d/sdf/functions.glsl"
#include "shader_parts/2d/primitives/primitives.glsl"

void main()
{
    outColor = texture(textures[textureIndex], UV) * vec4(1.0, 1.0, 1.0, 1.0);
    /*
    switch (instanceBuffer.objects[PushConstants.instanceIndex].type) {

        case TWOD_OBJ_TYPE_ROUNDED_RECTANGLE:
        RoundedRectangle();
        break;

        case TWOD_OBJ_TYPE_CIRCLE:
        Circle();
        break;

        case TWOD_OBJ_TYPE_BASIC_SPRITE:
        outColor = texture(Sampler2D, UV) * instanceBuffer.objects[PushConstants.instanceIndex].bgColor;
        break;

        case TWOD_OBJ_TYPE_BASIC_GLYPH:
        outColor = texture(Sampler2D, UV).r * instanceBuffer.objects[PushConstants.instanceIndex].bgColor;
        break;

        case TWOD_OBJ_TYPE_BASIC_RECTANGLE:
        outColor = instanceBuffer.objects[PushConstants.instanceIndex].bgColor;
        break;

        default :
        outColor = vec4(1.0, 0.0, 1.0, 1.0);
        break;
    }
    */
}
