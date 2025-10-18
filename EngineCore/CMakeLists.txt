cmake_minimum_required(VERSION 4.1)
project (EngineCore)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS "EngineCore/*.cpp" "EngineCore/*.h")

add_library(EngineCore STATIC ${ENGINE_SOURCES})

target_compile_definitions(EngineCore PRIVATE
    GLFW_INCLUDE_VULKAN
    GLM_FORCE_RADIANS
    GLM_FORCE_DEPTH_ZERO_TO_ONE
    YAML_CPP_STATIC_DEFINE
)

target_include_directories(EngineCore PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_precompile_headers(EngineCore PRIVATE "EngineCore/Pch.h")

target_link_libraries(EngineCore PUBLIC ExternalsLib)
