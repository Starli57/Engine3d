cmake_minimum_required(VERSION 4.1)
project (ResourcesConverter)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE CONVERTERR_SOURCES CONFIGURE_DEPENDS "*.cpp" "*.h")

add_executable(ResourcesConverter ${CONVERTERR_SOURCES})
#add_library(ResourcesConverter STATIC ${CONVERTERR_SOURCES})

target_compile_definitions(EngineCore PUBLIC
    TINYGLTF_IMPLEMENTATION
	STB_IMAGE_IMPLEMENTATION
	STB_IMAGE_WRITE_IMPLEMENTATION
	TINYGLTF_NOEXCEPTION
)

target_include_directories(ResourcesConverter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ResourcesConverter PUBLIC ExternalsLib)
target_link_libraries(ResourcesConverter PUBLIC EngineCore)