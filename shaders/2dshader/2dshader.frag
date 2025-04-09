#version 460
//layout(set = 0, binding = 2) uniform sampler2DArray textures;
layout(set = 0, binding = 2) uniform sampler2D atlas;

layout (location = 2) in vec2 inUV;

/*
layout (location = 2) in vec2 UV;
layout(location = 3) flat in int instanceIndex;
layout(location = 4) flat in int textureIndex;



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

*/

layout(location = 0) out vec4 outColor;

void main()
{
    //vec2 atlasUV = mix(instanceBuffer.objects[0].uvMin, instanceBuffer.objects[0].uvMax, inUV);
    outColor = vec4(0.0, 1.0, 1.0, 1.0);
    //outColor = vec4(1.0, 0.0, 1.0, 1.0); //texture(textures[textureIndex], UV) * vec4(1.0, 1.0, 1.0, 1.0);
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
