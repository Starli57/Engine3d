workspace "Engine3d_cpp"
	configurations { "Debug", "Release" }
	architecture "x86_64"
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

Includes = {}
Includes["GLFW"] = "Externals/Glfw/include"

Libs = {}

startproject "ExampleProject"

project "ExampleProject"
	location "ExampleProject"
	kind "ConsoleApp"
	language "C++"

	targetdir ("Output/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
		"Engine3d/Source"
	}


	links
	{
		"Engine3d"
	}
	
	cppdialect "C++20"
	staticruntime "On"
	systemversion "latest"

	filter "system:windows"
		defines
		{
			"ENGINE_WIN"
		}

	filter "configurations:Debug"
		defines
		{
			"DEBUG"
		}

	filter "configurations:Release"
		optimize "On"

project "Engine3d"
	location "Engine3d"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "On"
	systemversion "latest"


	targetdir ("Output/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "Pch.h"
	pchsource "%{prj.name}/Source/Pch.cpp"

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}
	
	includedirs
	{
		"%{Includes.GLFW}"
	}

	links
	{
		"GLFW"
	}
	
	filter "system:windows"
		defines
		{
			"ENGINE_WIN",
			"ENGINE_DLL_BUILD"
		}

		postbuildcommands { "copy $(SolutionDir)Output\\" .. outputdir .. "\\%{prj.name}\\%{prj.name}.dll $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject" }
		
	filter { "not system:windows" }
		postbuildcommands { "copy $(SolutionDir)Output/" .. outputdir .. "/%{prj.name}/%{prj.name}.dll $(SolutionDir)Output/" .. outputdir .. "/ExampleProject" }

	filter "configurations:Debug"
		defines
		{
			"DEBUG"
		}

	filter "configurations:Release"
		optimize "On"

project "GLFW"
	kind "StaticLib"
	language "C"
	
	targetdir ("Output/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"Externals/Glfw/src/**.h",
		"Externals/Glfw/src/**.c"
	}
		
	systemversion "latest"
	staticruntime "On"
	
	filter "system:windows"
		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "system:linux"
		defines
		{
			"_GLFW_X11"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"