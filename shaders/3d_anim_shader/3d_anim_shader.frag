#version 450

#include "shader_parts/common/push_constants.glsl"

#include "shader_parts/common/instance_data.glsl"

// Common
#include "shader_parts/common/object_types.glsl"
#include "shader_parts/common/frag_layout.glsl"

// 2D
#include "shader_parts/2d/sdf/functions.glsl"
#include "shader_parts/2d/primitives/primitives.glsl"

void main()
{
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
}
