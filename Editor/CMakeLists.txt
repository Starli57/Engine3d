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
		TARGET Editor POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
		"${folderPath}"
		"$<TARGET_FILE_DIR:Editor>/${folderName}"
        
		COMMENT "Copying ${folderName} folder"
    )
endforeach()