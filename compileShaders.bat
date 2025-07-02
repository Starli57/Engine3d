@echo off
set "ShaderPath=ExampleProject/Shaders"
@echo on

%VULKAN_SDK%/Bin/glslc.exe %ShaderPath%/Opaque.vert -o %ShaderPath%/OpaqueVert.spv
%VULKAN_SDK%/Bin/glslc.exe %ShaderPath%/Opaque.frag -o %ShaderPath%/OpaqueFrag.spv
%VULKAN_SDK%/Bin/glslc.exe %ShaderPath%/ShadowPass.vert -o %ShaderPath%/ShadowPassVert.spv
%VULKAN_SDK%/Bin/glslc.exe %ShaderPath%/ShadowPass.frag -o %ShaderPath%/ShadowPassFrag.spv


pause