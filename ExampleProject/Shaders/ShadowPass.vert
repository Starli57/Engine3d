#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUv;
layout(location = 3) in vec3 inColor;

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

layout(push_constant) uniform Model{
    mat4 model;
} uboM;

void main() 
{
    gl_Position = lights.viewProjection * uboM.model * vec4(inPosition, 1.0);
}