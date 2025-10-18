cmake_minimum_required(VERSION 4.1)
project (ExampleProject)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE EXAMPLE_PROJECT CONFIGURE_DEPENDS "*.cpp" "*.h")
#add_executable(ExampleProject ${EXAMPLE_PROJECT})
add_library(ExampleProject STATIC ${EXAMPLE_PROJECT})

target_include_directories(ExampleProject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ExampleProject PRIVATE ExternalsLib)
#target_link_libraries(ExampleProject PRIVATE Editor)
target_link_libraries(ExampleProject PRIVATE EngineCore)