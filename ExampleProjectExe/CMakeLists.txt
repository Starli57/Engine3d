cmake_minimum_required(VERSION 4.1)
project (ExampleProjectExe)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE EXAMPLE_PROJECT_EXE CONFIGURE_DEPENDS "*.cpp" "*.h")
add_executable(ExampleProjectExe ${EXAMPLE_PROJECT_EXE})

target_include_directories(ExampleProjectExe PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ExampleProjectExe PRIVATE ExampleProject)
target_link_libraries(ExampleProjectExe PRIVATE EngineCore)

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
		TARGET ExampleProjectExe POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
		"${folderPath}"
		"$<TARGET_FILE_DIR:ExampleProjectExe>/${folderName}"
        
		COMMENT "Copying ${folderName} folder"
    )
endforeach()