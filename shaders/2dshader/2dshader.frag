#version 460

layout(set = 0, binding = 2) uniform sampler2DArray textures;

// Instanced attributes
layout (location = 2) in vec3 inUV;
layout (location = 3) in vec4 inAtlasCoords;
layout (location = 4) in vec4 inColor;

layout(location = 0) out vec4 outColor;

void main()
{
    // outColor = texture(atlas, mix(inAtlasCoords.xy, inAtlasCoords.zw, inUV.xy)) * vec4(1.0, 1.0, 1.0, 1.0);

    if(inUV.z == -1.0f){
        outColor = inColor;
    } else {
        outColor = texture(textures, inUV) * inColor; 
    }

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
