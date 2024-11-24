workspace "Engine"
	configurations { "Debug", "Release" }
	architecture "x86_64"
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

VulkanSdk = os.getenv("VULKAN_SDK")
PythonSdk = os.getenv("PYTHON_SDK")

Externals = {}
Externals["Glfw"] = "Externals/GLFW"
Externals["Glm"] = "Externals/Glm"
Externals["SpdLog"] = "Externals/SpdLog"
Externals["Entt"] = "Externals/Entt"
Externals["Stb"] = "Externals/Stb"
Externals["TinyObjLoader"] = "Externals/TinyObjLoader"
Externals["TinyGltf"] = "Externals/TinyGltf"
Externals["DearImgui"] = "Externals/DearImgui"
Externals["YamlCpp"] = "Externals/YamlCpp"
Externals["Catch2"] = "Externals/Catch2"
Externals["Pybind11"] = "Externals/Pybind11"

Includes = {}
Includes["Editor"] = "Editor"
Includes["EngineCore"] = "EngineCore"
Includes["ExampleProject"] = "ExampleProject"

Includes["Glfw"] = "%{Externals.Glfw}/include"
Includes["Glm"] = "%{Externals.Glm}"
Includes["SpdLog"] = "%{Externals.SpdLog}/include"
Includes["Entt"] = "%{Externals.Entt}/single_include/entt"
Includes["Stb"] = "%{Externals.Stb}"
Includes["TinyObjLoader"] = "%{Externals.TinyObjLoader}"
Includes["TinyGltf"] = "%{Externals.TinyGltf}"
Includes["DearImgui"] = "%{Externals.DearImgui}"
Includes["YamlCpp"] = "%{Externals.YamlCpp}/include"
Includes["Catch2"] = "%{Externals.Catch2}/extras"
Includes["Pybind11"] = "%{Externals.Pybind11}/include"

Includes["Vulkan"] = "%{VulkanSdk}/Include"
Includes["Python"] = "%{PythonSdk}/include"

LibFolders = {}
LibFolders["Vulkan"] = "%{VulkanSdk}/Lib"
LibFolders["Python"] = "%{PythonSdk}/libs"

Libs = {}
Libs["Vulkan"] = "%{LibFolders.Vulkan}/vulkan-1.lib"
Libs["Python"] = "%{LibFolders.Python}/python312.lib"

startproject "ExampleProject"
		
project "ExampleProject"
	location "ExampleProject"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "Off"
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
		"%{Includes.Editor}",
		"%{Includes.EngineCore}",
		
		"%{Includes.Glfw}",
		"%{Includes.Glm}",
		"%{Includes.SpdLog}",
		"%{Includes.Entt}",
		"%{Includes.Stb}",
		"%{Includes.TinyObjLoader}",
		"%{Includes.TinyGltf}",
		"%{Includes.Pybind11}",

		"%{Includes.Vulkan}",
		"%{Includes.Python}"
	}
	
	links
	{
		"Editor",
		"EngineCore",

		"Glfw",
		"Glm",

		"%{Libs.Vulkan}",
		"%{Libs.Python}"
	}
	
	defines
	{
		"GLFW_INCLUDE_VULKAN",
		"GLM_FORCE_RADIANS",
		"GLM_FORCE_DEPTH_ZERO_TO_ONE"
	}

	filter "system:windows"
		defines
		{
			"PROJECT_WIN",
			"PROJECT_DLL_BUILD"
		}

		postbuildcommands { "mkdir $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject" }
		postbuildcommands { "mkdir $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject\\Shaders" }
		postbuildcommands { "mkdir $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject\\Resources\\Meshes" }
		postbuildcommands { "mkdir $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject\\Resources\\Textures" }

		postbuildcommands { "copy $(SolutionDir)\\ExampleProject\\Shaders $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject\\Shaders" }
		postbuildcommands { "copy $(SolutionDir)\\ExampleProject\\Resources\\Meshes $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject\\Resources\\Meshes" }
		postbuildcommands { "copy $(SolutionDir)\\ExampleProject\\Resources\\Textures $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject\\Resources\\Textures" }
		
	filter "not system:windows"
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

project "Editor"
	location "Editor"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "Off"
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
		
		"%{Includes.DearImgui}",
		"%{Includes.DearImgui}/backends",
		"%{Includes.Glfw}",
		"%{Includes.Glm}",
		"%{Includes.SpdLog}",
		"%{Includes.Entt}",
		"%{Includes.Stb}",
		"%{Includes.TinyObjLoader}",
		"%{Includes.TinyGltf}",
		"%{Includes.YamlCpp}",
		"%{Includes.Pybind11}",

		"%{Includes.Vulkan}",
		"%{Includes.Python}"
	}
	
	links
	{
		"EngineCore",

		"DearImgui",
		"YamlCpp",
		
		"%{Libs.Python}"
	}
	
	defines
	{
		"GLFW_INCLUDE_VULKAN",
		"GLM_FORCE_RADIANS",
		"GLM_FORCE_DEPTH_ZERO_TO_ONE",
		"YAML_CPP_STATIC_DEFINE"
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

project "EngineCore"
	location "EngineCore"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "Off"
	systemversion "latest"

	targetdir ("Output/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "EngineCore/Pch.h"
	pchsource "%{prj.name}/EngineCore/Pch.cpp"

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
	}
	
	includedirs
	{
		"%{prj.name}",

		"%{Includes.Glfw}",
		"%{Includes.Glm}",
		"%{Includes.SpdLog}",
		"%{Includes.Entt}",
		"%{Includes.Stb}",
		"%{Includes.TinyObjLoader}",
		"%{Includes.TinyGltf}",
		"%{Includes.YamlCpp}",
		"%{Includes.Pybind11}",
		
		"%{Includes.Vulkan}",
		"%{Includes.Python}"
	}

	links
	{
		"Glfw",
		"Glm",
		"YamlCpp",

		"%{Libs.Vulkan}",
		"%{Libs.Python}"
	}
	
	defines
	{
		"GLFW_INCLUDE_VULKAN",
		"GLM_FORCE_RADIANS",
		"GLM_FORCE_DEPTH_ZERO_TO_ONE",
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
	staticruntime "Off"
	
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

		"%{Includes.Catch2}",
		"%{Includes.Glfw}",
		"%{Includes.Glm}",
		"%{Includes.SpdLog}",
		"%{Includes.Entt}",
		"%{Includes.Stb}",
		"%{Includes.TinyObjLoader}",
		"%{Includes.YamlCpp}",
		"%{Includes.Pybind11}",

		"%{Includes.Vulkan}"
	}

	links
	{
		"EngineCore",
		"Catch2",
		"Glfw",
		"Glm",
		"YamlCpp",

		"%{Libs.Vulkan}"
	}
	
	filter "system:windows"
		defines
		{
			"PROJECT_WIN",
			"PROJECT_DLL_BUILD"
		}

project "Glfw"
	kind "StaticLib"
	language "C"
	systemversion "latest"
	staticruntime "Off"
	
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
	cppdialect "C++20"
	systemversion "latest"
	staticruntime "Off"
	
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
	staticruntime "Off"
	
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
	staticruntime "Off"
	
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
	staticruntime "Off"
	
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