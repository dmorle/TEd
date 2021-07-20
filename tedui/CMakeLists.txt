
# tedui

cmake_minimum_required (VERSION 3.8)

include_directories("include")
include_directories("${PROJECT_SOURCE_DIR}/tedcore/include")

add_compile_definitions(TEDC_DIRECT2D)
add_library(
	tedui SHARED
	"src/tedui.cpp"
	
	"include/tedui/tedui.hpp"
)

set_property(TARGET tedui PROPERTY LINKER_LANGUAGE CXX)
set_property(TARGET tedui PROPERTY CXX_STANDARD 20)

target_include_directories(tedui PUBLIC "${PROJECT_SOURCE_DIR}/tedcore/include")

target_link_libraries(tedui libtedc)

add_custom_command(
	TARGET tedui
	COMMAND "${CMAKE_COMMAND}"
	ARGS "-E" "copy" "${CMAKE_CURRENT_BINARY_DIR}/tedui.dll" "${CMAKE_BINARY_DIR}/tedcore/"
	DEPENDS libtedc
)
add_custom_command(
	TARGET tedui
	COMMAND "${CMAKE_COMMAND}"
	ARGS "-E" "copy" "${CMAKE_CURRENT_BINARY_DIR}/tedui.pdb" "${CMAKE_BINARY_DIR}/tedcore/"
	DEPENDS libtedc
)
add_custom_command(
	TARGET tedui
	COMMAND "${CMAKE_COMMAND}"
	ARGS "-E" "copy" "${CMAKE_CURRENT_SOURCE_DIR}/tedui.ted" "${CMAKE_BINARY_DIR}/tedcore/extensions/"
	DEPENDS libtedc
)
