@echo off
set "ShaderPath=Renderer/Code/Renderer/Shaders"
@echo on

%VULKAN_SDK%/Bin/glslc.exe %ShaderPath%/Vert.vert -o %ShaderPath%/vert.spv
%VULKAN_SDK%/Bin/glslc.exe %ShaderPath%/Frag.frag -o %ShaderPath%/frag.spv
pause