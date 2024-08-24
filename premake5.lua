workspace "Engine3d_cpp"
	configurations { "Debug", "Release" }
	architecture "x86_64"
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

VulkanSdk = os.getenv("VULKAN_SDK")

Externals = {}
Externals["Glfw"] = "Externals/GLFW"
Externals["Glm"] = "Externals/Glm"
Externals["SpdLog"] = "Externals/SpdLog"
Externals["Entt"] = "Externals/Entt"
Externals["Stb"] = "Externals/Stb"
Externals["TinyObjLoader"] = "Externals/TinyObjLoader"
Externals["DearImgui"] = "Externals/DearImgui"
Externals["YamlCpp"] = "Externals/YamlCpp"
Externals["Catch2"] = "Externals/Catch2"

Includes = {}
Includes["EngineCore"] = "EngineCore"
Includes["ExampleProject"] = "ExampleProject"

Includes["Glfw"] = "%{Externals.Glfw}/include"
Includes["Glm"] = "%{Externals.Glm}"
Includes["Vulkan"] = "%{VulkanSdk}/Include"
Includes["SpdLog"] = "%{Externals.SpdLog}/include"
Includes["Entt"] = "%{Externals.Entt}/single_include/entt"
Includes["Stb"] = "%{Externals.Stb}"
Includes["TinyObjLoader"] = "%{Externals.TinyObjLoader}"
Includes["DearImgui"] = "%{Externals.DearImgui}"
Includes["YamlCpp"] = "%{Externals.YamlCpp}/include"
Includes["Catch2"] = "%{Externals.Catch2}/extras"

LibFolders = {}
LibFolders["Vulkan"] = "%{VulkanSdk}/Lib"

Libs = {}
Libs["Vulkan"] = "%{LibFolders.Vulkan}/vulkan-1.lib"

startproject "Editor"
		
project "Editor"
	location "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "On"
	systemversion "latest"

	targetdir ("Output/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
	}

	includedirs
	{
		"%{prj.name}",
		"%{Includes.EngineCore}",
		"%{Includes.ExampleProject}",
		
		"%{Includes.DearImgui}",
		"%{Includes.DearImgui}/backends",
		"%{Includes.Glfw}",
		"%{Includes.Glm}",
		"%{Includes.Vulkan}",
		"%{Includes.SpdLog}",
		"%{Includes.Entt}",
		"%{Includes.Stb}",
		"%{Includes.TinyObjLoader}",
		"%{Includes.YamlCpp}"
	}
	
	links
	{
		"EngineCore",
		"ExampleProject",

		"DearImgui",
		"YamlCpp"
	}
	
	defines
	{
		"GLFW_INCLUDE_VULKAN",
		"GLM_FORCE_RADIANS",
		"GLM_FORCE_DEPTH_ZERO_TO_ONE",
		"STB_IMAGE_IMPLEMENTATION",
		"TINYOBJLOADER_IMPLEMENTATION",
		"YAML_CPP_STATIC_DEFINE"
	}

	filter "system:windows"
		defines
		{
			"PROJECT_WIN"
		}

	filter "configurations:Debug"
		defines
		{
			"DEBUG"
		}

	filter "configurations:Release"
		optimize "On"

project "EngineCore"
	location "EngineCore"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "On"
	systemversion "latest"

	targetdir ("Output/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "EngineCore/Pch.h"
	pchsource "%{prj.name}/EngineCore/Pch.cpp"

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp",
	}
	
	includedirs
	{
		"%{prj.name}",

		"%{Includes.Glfw}",
		"%{Includes.Glm}",
		"%{Includes.Vulkan}",
		"%{Includes.SpdLog}",
		"%{Includes.Entt}",
		"%{Includes.Stb}",
		"%{Includes.TinyObjLoader}",
		"%{Includes.YamlCpp}"
	}

	links
	{
		"Glfw",
		"Glm",
		"%{Libs.Vulkan}",
		"YamlCpp"
	}
	
	defines
	{
		"GLFW_INCLUDE_VULKAN",
		"GLM_FORCE_RADIANS",
		"GLM_FORCE_DEPTH_ZERO_TO_ONE",
		"STB_IMAGE_IMPLEMENTATION",
		"TINYOBJLOADER_IMPLEMENTATION",
		"YAML_CPP_STATIC_DEFINE"
	}
	
	prebuildcommands 
	{
		"cd $(SolutionDir)",
		"call compileShaders.bat"
	} 

	filter "system:windows"
		defines
		{
			"PROJECT_WIN"
		}
		postbuildcommands { "mkdir $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject" }
		postbuildcommands { "mkdir $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject\\Shaders" }
		postbuildcommands { "mkdir $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject\\Resources\\Meshes" }
		postbuildcommands { "mkdir $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject\\Resources\\Textures" }

		postbuildcommands { "copy $(SolutionDir)\\ExampleProject\\Shaders $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject\\Shaders" }
		postbuildcommands { "copy $(SolutionDir)\\ExampleProject\\Resources\\Meshes $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject\\Resources\\Meshes" }
		postbuildcommands { "copy $(SolutionDir)\\ExampleProject\\Resources\\Textures $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject\\Resources\\Textures" }
		
	filter { "not system:windows" }
		postbuildcommands { "mkdir $(SolutionDir)Output/" .. outputdir .. "/ExampleProject" }
		postbuildcommands { "mkdir $(SolutionDir)Output/" .. outputdir .. "/ExampleProject/Shaders" }
		postbuildcommands { "mkdir $(SolutionDir)Output/" .. outputdir .. "/ExampleProject/Resources/Meshes" }
		postbuildcommands { "mkdir $(SolutionDir)Output/" .. outputdir .. "/ExampleProject/Resources/Textures" }

		postbuildcommands { "copy $(SolutionDir)/ExampleProject/Shaders $(SolutionDir)Output/" .. outputdir .. "/ExampleProject/Shaders" }
		postbuildcommands { "copy $(SolutionDir)/ExampleProject/Resources/Meshes $(SolutionDir)Output/" .. outputdir .. "/ExampleProject/Resources/Meshes" }
		postbuildcommands { "copy $(SolutionDir)/ExampleProject/Resources/Textures $(SolutionDir)Output/" .. outputdir .. "/ExampleProject/Resources/Textures" }

	filter "configurations:Debug"
		defines
		{
			"DEBUG"
		}

	filter "configurations:Release"
		optimize "On"
		
project "ExampleProject"
	location "ExampleProject"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "On"
	systemversion "latest"

	targetdir ("Output/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
	}

	includedirs
	{
		"%{prj.name}",
		"%{Includes.EngineCore}",
		
		"%{Includes.Glfw}",
		"%{Includes.Glm}",
		"%{Includes.Vulkan}",
		"%{Includes.SpdLog}",
		"%{Includes.Entt}",
		"%{Includes.Stb}",
		"%{Includes.TinyObjLoader}"
	}
	
	links
	{
		"Glfw",
		"Glm",
		"%{Libs.Vulkan}"
	}
	
	defines
	{
		"GLFW_INCLUDE_VULKAN",
		"GLM_FORCE_RADIANS",
		"GLM_FORCE_DEPTH_ZERO_TO_ONE",
		"STB_IMAGE_IMPLEMENTATION",
		"TINYOBJLOADER_IMPLEMENTATION"
	}

	filter "system:windows"
		defines
		{
			"PROJECT_WIN",
			"PROJECT_DLL_BUILD"
		}

	filter "configurations:Debug"
		defines
		{
			"DEBUG"
		}

	filter "configurations:Release"
		optimize "On"

project "Tests"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	systemversion "latest"
	staticruntime "On"
	
	targetdir ("Output/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
	}

	includedirs
	{
		"%{prj.name}",
		"%{Includes.Catch2}"
	}

	links
	{
		"Catch2"
	}

project "Glfw"
	kind "StaticLib"
	language "C"
	systemversion "latest"
	staticruntime "On"
	
	targetdir ("Output/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{Externals.Glfw}/src/**.h",
		"%{Externals.Glfw}/src/**.c"
	}
	
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

		
project "Glm"
	kind "StaticLib"
	language "C++"
	systemversion "latest"
	staticruntime "On"
	
	targetdir ("Output/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{Includes.Glm}/glm/**.hpp",
		"%{Includes.Glm}/glm/**.cpp",
		"%{Includes.Glm}/glm/**.inl"
	}

	includedirs
	{
		"%{Includes.Glm}"
	}
		
project "DearImgui"
	kind "StaticLib"
	language "C++"
	systemversion "latest"
	staticruntime "On"
	
	targetdir ("Output/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{Includes.DearImgui}/*.h",
		"%{Includes.DearImgui}/*.cpp",
		"%{Includes.DearImgui}/backends/imgui_impl_glfw.cpp",
		"%{Includes.DearImgui}/backends/imgui_impl_glfw.h",
		"%{Includes.DearImgui}/backends/imgui_impl_vulkan.cpp",
		"%{Includes.DearImgui}/backends/imgui_impl_vulkan.h"
	}

	includedirs
	{
		"%{Includes.Glfw}",
		"%{Includes.DearImgui}",
		"%{Includes.DearImgui}/backends",
		"%{Includes.Vulkan}"
	}
	
	links
	{
		"Glfw",
		"%{Libs.Vulkan}"
	}
	
	defines
	{
		"GLFW_INCLUDE_VULKAN"
	}

			
project "YamlCpp"
	kind "StaticLib"
	language "C++"
	systemversion "latest"
	staticruntime "On"
	
	targetdir ("Output/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{Externals.YamlCpp}/src/*.h",
		"%{Externals.YamlCpp}/src/*.cpp"
	}

	includedirs
	{
		"%{Includes.YamlCpp}"
	}

	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}

project "Catch2"
	kind "StaticLib"
	language "C++"
	systemversion "latest"
	staticruntime "On"
	
	targetdir ("Output/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{Externals.Catch2}/extras/*.hpp",
		"%{Externals.Catch2}/extras/*.cpp"
	}

	includedirs
	{
		"%{Includes.Catch2}"
	}