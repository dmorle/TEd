
# tedcore

cmake_minimum_required (VERSION 3.8)


add_library(
	libtedc SHARED
	"src/tedcore.cpp"
	"src/backend/impl.cpp"
	
	"include/tedcore/tedcore.hpp"
	"include/tedcore/backend/impl.hpp"
)

set_property(TARGET libtedc PROPERTY LINKER_LANGUAGE CXX)
set_property(TARGET libtedc PROPERTY CXX_STANDARD 17)

target_include_directories(libtedc PRIVATE "include")
target_include_directories(libtedc PRIVATE "${PROJECT_SOURCE_DIR}/tedlang/include")

target_link_libraries(libtedc PRIVATE libtedl)

# linking the backend function implementations
if(WIN32)

	add_executable(
		ted WIN32
		"src/backend/windows/main.cpp"
	)
	
	set_property(TARGET ted PROPERTY CXX_STANDARD 17)
	target_include_directories(ted PRIVATE "include")
	target_link_libraries(ted PRIVATE libtedc)

else()
	message(FATAL_ERROR "TEd currently only supports windows")
endif()
