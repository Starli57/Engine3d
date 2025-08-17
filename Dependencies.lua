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