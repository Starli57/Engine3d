@echo off
set "ShaderPath=ExampleProject/Shaders"
@echo on

%VULKAN_SDK%/Bin/glslc.exe %ShaderPath%/Albedo.vert -o %ShaderPath%/AlbedoVert.spv
%VULKAN_SDK%/Bin/glslc.exe %ShaderPath%/Albedo.frag -o %ShaderPath%/AlbedoFrag.spv

pause