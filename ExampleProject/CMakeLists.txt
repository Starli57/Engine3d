cmake_minimum_required(VERSION 4.1)
project (ExampleProject)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE EXAMPLE_PROJECT CONFIGURE_DEPENDS "*.cpp" "*.h")
add_executable(ExampleProject ${EXAMPLE_PROJECT})

target_include_directories(ExampleProject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ExampleProject PRIVATE ExternalsLib)
target_link_libraries(ExampleProject PRIVATE Editor)
target_link_libraries(ExampleProject PRIVATE EngineCore)

set(userProject "${CMAKE_SOURCE_DIR}/ExampleProject")

set(foldersToCopy 
	"${userProject}/Resources"
	"${userProject}/Shaders"
	"${userProject}/Worlds"
	"${userProject}/Prefabs"
	"${CMAKE_SOURCE_DIR}/Externals/Fonts"
)

foreach(folderPath ${foldersToCopy})
	get_filename_component(folderName ${folderPath} NAME)
	add_custom_command(
		TARGET ExampleProject POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
		"${folderPath}"
		"$<TARGET_FILE_DIR:ExampleProject>/${folderName}"
        
		COMMENT "Copying ${folderName} folder"
    )
endforeach()