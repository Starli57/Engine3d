cmake_minimum_required(VERSION 4.1)
project (Editor)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS "*.cpp" "*.h")

add_executable(Editor ${EDITOR_SOURCES})
#add_library(Editor STATIC ${EDITOR_SOURCES})

target_include_directories(Editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Editor PUBLIC ExternalsLib)
target_link_libraries(Editor PUBLIC EngineCore)
target_link_libraries(Editor PUBLIC ExampleProject)