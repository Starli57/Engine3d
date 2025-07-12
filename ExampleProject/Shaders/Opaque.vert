#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec3 inBitangent;
layout(location = 4) in vec2 inUv;
layout(location = 5) in vec3 inColor;

layout(binding = 0) uniform World 
{
    mat4 cameraView;
    mat4 cameraProj;
    vec3 cameraPosition;
} world;

layout(binding = 1) uniform Lights 
{
    mat4 viewProjection;
    vec3 inLightPosition;
    vec3 inLightDirection;
} lights;

//layout(binding = 3) uniform Material
//{
//    float transparency;
//    float roughness;
//    float metallic;
//    float sheen;
//    float specularExponent;
//    float indexOfRefraction;
//
//    vec3 ambientColor;
//    vec3 diffuseColor;
//    vec3 specularColor;
//    vec3 emissionColor;
//} material;

layout(push_constant) uniform Model{
    mat4 model;
} uboM;

layout(location = 0) out vec3 outWorldPosition;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec2 outUv;
layout(location = 3) out vec3 outVertexColor;
layout(location = 4) out vec3 outLightPosition;
layout(location = 5) out vec3 outLightDirection;
layout(location = 6) out vec3 outViewPosition;
layout(location = 7) out vec4 outFragPosLightSpace;
layout(location = 8) out mat4 outLightMatrix;
layout(location = 12) out mat3 outTBN;

void main() 
{
    outWorldPosition = vec3(uboM.model * vec4(inPosition, 1.0));
    gl_Position = world.cameraProj * world.cameraView * vec4(outWorldPosition, 1.0);

    outNormal = mat3(transpose(inverse(uboM.model))) * inNormal;
    outUv = inUv;
    outVertexColor = inColor;
    outLightPosition = lights.inLightPosition;
    outLightDirection = lights.inLightDirection;
    outViewPosition = world.cameraPosition;
    outLightMatrix = lights.viewProjection;
    outFragPosLightSpace = outLightMatrix * vec4(outWorldPosition, 1.0);
    
    vec3 T = normalize(vec3(uboM.model * vec4(inTangent,   0.0)));
    vec3 B = normalize(vec3(uboM.model * vec4(inBitangent, 0.0)));
    vec3 N = normalize(vec3(uboM.model * vec4(inNormal,    0.0)));
    outTBN = mat3(T, B, N);
}