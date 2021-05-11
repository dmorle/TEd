
# tedcore

cmake_minimum_required (VERSION 3.8)


add_library(
	libtedcore STATIC
	"src/tedcore.cpp"

	"include/tedcore/tedcore.hpp"
)

set_property(TARGET libtedcore PROPERTY LINKER_LANGUAGE CXX)
set_property(TARGET libtedcore PROPERTY CXX_STANDARD 17)

target_include_directories(libtedcore PRIVATE "include")
target_include_directories(libtedcore PRIVATE "${PROJECT_SOURCE_DIR}/tedlang/include")

target_link_libraries(libtedcore PRIVATE libtedl)

# linking the backend function implementations
if(WIN32)

	add_executable(
		ted WIN32
		"src/backend/windows/main.cpp"
		"src/backend/windows/ted.cpp"

		"include/tedcore/backend/windows/ted.hpp"
	)

	set_property(TARGET libtedcore PROPERTY LINKER_LANGUAGE CXX)
	set_property(TARGET ted PROPERTY CXX_STANDARD 17)

	target_include_directories(ted PRIVATE "include")

	target_link_libraries(ted PRIVATE libtedcore)

else()
	message(FATAL_ERROR "TEd currently only supports windows")
endif()
