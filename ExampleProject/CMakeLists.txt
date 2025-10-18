cmake_minimum_required(VERSION 4.1)
project (ExampleProject)
set(CMAKE_CXX_STANDARD 20)

add_executable(ExampleProject ExampleProject/Main.cpp)

target_link_libraries(ExampleProject PRIVATE ExternalsLib)
target_link_libraries(ExampleProject PRIVATE Editor)
target_link_libraries(ExampleProject PRIVATE EngineCore)