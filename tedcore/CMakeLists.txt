
# tedcore

cmake_minimum_required (VERSION 3.8)
include("${PROJECT_SOURCE_DIR}/utils.cmake")

include_directories("include")

# libtedc

add_compile_definitions(TEDC_DIRECT2D)
add_library(
	libtedc SHARED
	"src/tedcore.cpp"
	"src/backend/impl.cpp"
	"src/backend/api.cpp"
	"src/backend/tedlapi/brush.cpp"
	"src/backend/tedlapi/rect.cpp"
	"src/backend/tedlapi/line.cpp"
	
	"include/tedcore/tedcore.hpp"
	"include/tedcore/backend/impl.hpp"
	"include/tedcore/backend/tedlapi/brush.hpp"
	"include/tedcore/backend/tedlapi/rect.hpp"
	"include/tedcore/backend/tedlapi/line.hpp"
)

set_property(TARGET libtedc PROPERTY LINKER_LANGUAGE CXX)
set_property(TARGET libtedc PROPERTY CXX_STANDARD 20)

target_include_directories(libtedc PUBLIC "${PROJECT_SOURCE_DIR}/tedlang/include")

target_link_libraries(libtedc libtedl)
target_link_libraries(libtedc D2d1)

add_custom_command(
	TARGET libtedc
	COMMAND "${CMAKE_COMMAND}"
	ARGS "-E" "copy" "${CMAKE_BINARY_DIR}/tedlang/libtedl.dll" "${CMAKE_CURRENT_BINARY_DIR}"
	DEPENDS libtedl
)
add_custom_command(
	TARGET libtedc
	COMMAND "${CMAKE_COMMAND}"
	ARGS "-E" "copy_directory" "${CMAKE_BINARY_DIR}/tedlang/slib/" "${CMAKE_CURRENT_BINARY_DIR}/slib/"
	DEPENDS libtedl io
)

# Copying test extensions
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/extensions")
# merge_dir("${CMAKE_CURRENT_BINARY_DIR}/extensions" "${PROJECT_SOURCE_DIR}/tedcore/extensions")
add_custom_command(
	TARGET libtedc
	COMMAND "${CMAKE_COMMAND}"
	ARGS "-E" "copy_directory" "${CMAKE_CURRENT_SOURCE_DIR}/extensions" "${CMAKE_CURRENT_BINARY_DIR}/extensions"
)

# linking the backend function implementations
if(WIN32)
	add_executable(
		ted WIN32
		"src/backend/windows/main.cpp"
		"src/backend/windows/ted.cpp"
		"include/tedcore/backend/windows/ted.hpp"
	)

	target_link_libraries(ted libtedc)
	target_link_libraries(libtedc D2d1)
	
	set_property(TARGET ted PROPERTY LINKER_LANGUAGE CXX)
	set_property(TARGET ted PROPERTY CXX_STANDARD 20)

else()
	message(FATAL_ERROR "TEd currently only supports windows")
endif()
