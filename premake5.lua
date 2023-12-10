workspace "Engine3d_cpp"
	configurations { "Debug", "Release" }
	architecture "x86_64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Engine3d"
	location "Engine3d"
	kind "SharedLib"
	language "C++"

	targetdir ("Output/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"ENGINE_WIN",
			"ENGINE_DLL_BUILD"
		}

		postbuildcommands { "copy $(SolutionDir)Output\\" .. outputdir .. "\\%{prj.name}\\%{prj.name}.dll $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject" }

	filter "configurations:Debug"
		defines
		{
			"DEBUG"
		}

	filter "configurations:Release"
		optimize "On"


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

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

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