#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(binding = 0) uniform ViewProjection {
    mat4 view;
    mat4 proj;
} uboViewProjection;

layout(binding = 1) uniform Model {
    mat4 model;
} uboModel;

layout(location = 0) out vec3 fragColor;

void main() 
{
    gl_Position = uboViewProjection.proj * uboViewProjection.view * uboModel.model * vec4(inPosition, 1.0);
    fragColor = inColor;
}