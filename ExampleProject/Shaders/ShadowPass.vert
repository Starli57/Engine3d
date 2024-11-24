#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUv;
layout(location = 3) in vec3 inColor;

layout(binding = 0) uniform ViewProjection {
    mat4 view;
    mat4 proj;
} uboVP;

layout(push_constant) uniform Model{
    mat4 model;
} uboM;

void main() 
{
    gl_Position = uboVP.proj * uboVP.view * uboM.model * vec4(inPosition, 1.0);
}