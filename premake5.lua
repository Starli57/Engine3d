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

Includes = {}
Includes["Glfw"] = "%{Externals.Glfw}/include"
Includes["Glm"] = "%{Externals.Glm}"
Includes["Vulkan"] = "%{VulkanSdk}/Include"
Includes["SpdLog"] = "%{Externals.SpdLog}/include"
Includes["Entt"] = "%{Externals.Entt}/single_include/entt"
Includes["Stb"] = "%{Externals.Stb}"
Includes["TinyObjLoader"] = "%{Externals.TinyObjLoader}"
Includes["DearImgui"] = "%{Externals.DearImgui}"

LibFolders = {}
LibFolders["Vulkan"] = "%{VulkanSdk}/Lib"

Libs = {}
Libs["Vulkan"] = "%{LibFolders.Vulkan}/vulkan-1.lib"

startproject "ExampleProject"

project "ExampleProject"
	location "ExampleProject"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "On"
	systemversion "latest"

	targetdir ("Output/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
		"%{Includes.Glfw}",
		"%{Includes.Glm}",
		"%{Includes.Vulkan}",
		"%{Includes.SpdLog}",
		"%{Includes.Entt}",
		"%{Includes.Stb}",
		"%{Includes.TinyObjLoader}",
		"Engine3d/Source"
	}
	
	links
	{
		"Engine3d"
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
			"ENGINE_WIN"
		}

	filter "configurations:Debug"
		defines
		{
			"DEBUG"
		}

	filter "configurations:Release"
		optimize "On"

		
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
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
		
		"%{Includes.DearImgui}/imconfig.h",
		"%{Includes.DearImgui}/imgui.cpp",
		"%{Includes.DearImgui}/imgui.h",
		"%{Includes.DearImgui}/imgui_draw.cpp",
		"%{Includes.DearImgui}/imgui_internal.h",
		"%{Includes.DearImgui}/imgui_tables.cpp",
		"%{Includes.DearImgui}/imgui_widgets.cpp",
		"%{Includes.DearImgui}/imstb_rectpack.h",
		"%{Includes.DearImgui}/imstb_textedit.h",
		"%{Includes.DearImgui}/imstb_truetype.h",
		
		"%{Includes.DearImgui}/backends/imgui_impl_vulkan.cpp",
		"%{Includes.DearImgui}/backends/imgui_impl_vulkan.h"
	}

	includedirs
	{
		"%{prj.name}/Source",
		"Engine3d/Source",
		"%{Includes.DearImgui}"
	}
	
	links
	{
		"Engine3d",
		"DearImgui"
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
		"%{prj.name}/Source/**.cpp",
	}
	
	includedirs
	{
		"%{prj.name}/Source",
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
	
	prebuildcommands 
	{
		"cd $(SolutionDir)",
		"call compileShaders.bat"
	} 

	filter "system:windows"
		defines
		{
			"ENGINE_WIN",
			"ENGINE_DLL_BUILD"
		}
		
		postbuildcommands { "mkdir $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject" }
		postbuildcommands { "mkdir $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject\\Shaders" }
		postbuildcommands { "mkdir $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject\\Meshes" }
		postbuildcommands { "mkdir $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject\\Textures" }
		postbuildcommands { "copy $(SolutionDir)Output\\" .. outputdir .. "\\%{prj.name}\\%{prj.name}.dll $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject" }
		postbuildcommands { "copy $(SolutionDir)\\ExampleProject\\Shaders $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject\\Shaders" }
		postbuildcommands { "copy $(SolutionDir)\\ExampleProject\\Meshes $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject\\Meshes" }
		postbuildcommands { "copy $(SolutionDir)\\ExampleProject\\Textures $(SolutionDir)Output\\" .. outputdir .. "\\ExampleProject\\Textures" }
		
	filter { "not system:windows" }
		postbuildcommands { "mkdir $(SolutionDir)Output/" .. outputdir .. "/ExampleProject" }
		postbuildcommands { "mkdir $(SolutionDir)Output/" .. outputdir .. "/ExampleProject/Shaders" }
		postbuildcommands { "mkdir $(SolutionDir)Output/" .. outputdir .. "/ExampleProject/Meshes" }
		postbuildcommands { "mkdir $(SolutionDir)Output/" .. outputdir .. "/ExampleProject/Textures" }
		postbuildcommands { "copy $(SolutionDir)Output/" .. outputdir .. "/%{prj.name}/%{prj.name}.dll $(SolutionDir)Output/" .. outputdir .. "/ExampleProject" }
		postbuildcommands { "copy $(SolutionDir)/ExampleProject/Shaders $(SolutionDir)Output/" .. outputdir .. "/ExampleProject/Shaders" }
		postbuildcommands { "copy $(SolutionDir)/ExampleProject/Meshes $(SolutionDir)Output/" .. outputdir .. "/ExampleProject/Meshes" }
		postbuildcommands { "copy $(SolutionDir)/ExampleProject/Textures $(SolutionDir)Output/" .. outputdir .. "/ExampleProject/Textures" }

	filter "configurations:Debug"
		defines
		{
			"DEBUG"
		}

	filter "configurations:Release"
		optimize "On"

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
		"%{Includes.DearImgui}/imconfig.h",
		"%{Includes.DearImgui}/imgui.cpp",
		"%{Includes.DearImgui}/imgui.h",
		"%{Includes.DearImgui}/imgui_draw.cpp",
		"%{Includes.DearImgui}/imgui_internal.h",
		"%{Includes.DearImgui}/imgui_tables.cpp",
		"%{Includes.DearImgui}/imgui_widgets.cpp",
		"%{Includes.DearImgui}/imstb_rectpack.h",
		"%{Includes.DearImgui}/imstb_textedit.h",
		"%{Includes.DearImgui}/imstb_truetype.h",
		
		"%{Includes.DearImgui}/backends/imgui_impl_vulkan.cpp",
		"%{Includes.DearImgui}/backends/imgui_impl_vulkan.h"

	}

	includedirs
	{
		"%{Includes.DearImgui}",
		"%{Includes.Vulkan}"
	}
	
	links
	{
		"%{Libs.Vulkan}"
	}
	
	defines
	{
		"GLFW_INCLUDE_VULKAN"
	}