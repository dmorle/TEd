
# tedcore

cmake_minimum_required (VERSION 3.8)

include_directories("include")

# libtedc

add_library(
	libtedc SHARED
	"src/tedcore.cpp"
	"src/backend/impl.cpp"
	"src/backend/api.cpp"
	
	"include/tedcore/tedcore.hpp"
	"include/tedcore/backend/impl.hpp"
	
)

set_property(TARGET libtedc PROPERTY LINKER_LANGUAGE CXX)
set_property(TARGET libtedc PROPERTY CXX_STANDARD 20)

target_include_directories(libtedc PUBLIC "${PROJECT_SOURCE_DIR}/tedlang/include")

target_link_libraries(libtedc libtedl)

# linking the backend function implementations
if(WIN32)
	add_executable(
		ted WIN32
		"src/backend/windows/main.cpp"
		"src/backend/windows/ted.cpp"
		"include/tedcore/backend/windows/ted.hpp"
	)
	target_link_libraries(ted libtedc)
	
	set_property(TARGET ted PROPERTY LINKER_LANGUAGE CXX)
	set_property(TARGET ted PROPERTY CXX_STANDARD 20)

else()
	message(FATAL_ERROR "TEd currently only supports windows")
endif()
