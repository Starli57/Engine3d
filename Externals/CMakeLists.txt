cmake_minimum_required(VERSION 4.1)
project(ExternalDependencies)
set(CMAKE_CXX_STANDARD 23)

find_package(Vulkan REQUIRED)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_LIST_DIR}/Externals/Stb)

set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)

add_subdirectory(spdlog)
add_subdirectory(glm)
add_subdirectory(Catch2)
add_subdirectory(Entt)
add_subdirectory(GLFW)
add_subdirectory(TinyGltf)
add_subdirectory(TinyObjLoader)
add_subdirectory(YamlCpp)

file(GLOB IMGUI_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/DearImgui/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/DearImgui/backends/imgui_impl_glfw.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/DearImgui/backends/imgui_impl_vulkan.cpp"
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/DearImgui"
    "${CMAKE_CURRENT_LIST_DIR}/DearImgui/backends"
)


add_library(ExternalsLib INTERFACE)

target_link_libraries(imgui PUBLIC 
	glfw
	Vulkan::Vulkan
)

target_link_libraries(ExternalsLib INTERFACE 
	spdlog::spdlog
	glm
	Catch2
	EnTT::EnTT
	glfw
	tinygltf
	tinyobjloader
	yaml-cpp
	stb
	imgui
)