#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUv;
layout(location = 3) in vec3 inColor;

layout(binding = 0) uniform ViewProjection {
    mat4 view;
    mat4 proj;
} uboVP;

layout(binding = 1) uniform Lights {
    mat4 viewProjection;
    vec3 inLightPosition;
    vec3 inLightDirection;
} lights;

layout(binding = 2) uniform Camera {
    vec3 viewPosition;
} camera;

layout(push_constant) uniform Model{
    mat4 model;
} uboM;

layout(location = 0) out vec3 outLocalPosition;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec2 outUv;
layout(location = 3) out vec3 outColor;
layout(location = 4) out vec3 outLightPosition;
layout(location = 5) out vec3 outLightDirection;
layout(location = 6) out vec3 outViewPosition;
layout(location = 7) out vec4 outFragPosLightSpace;
layout(location = 8) out mat4 outLightMatrix;

void main() 
{
    outLocalPosition = vec3(uboM.model * vec4(inPosition, 1.0));
    gl_Position = uboVP.proj * uboVP.view * vec4(outLocalPosition, 1.0);

    outNormal = mat3(transpose(inverse(uboM.model))) * inNormal;
    outUv = inUv;
    outColor = inColor;
    outLightPosition = lights.inLightPosition;
    outLightDirection = lights.inLightDirection;
    outViewPosition = camera.viewPosition;
    outLightMatrix = lights.viewProjection;
    outFragPosLightSpace = outLightMatrix * vec4(outLocalPosition, 1.0);
}