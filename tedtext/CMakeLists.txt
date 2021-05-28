
# tedtext

cmake_minimum_required (VERSION 3.8)

include("${PROJECT_SOURCE_DIR}/utils.cmake")

include_directories("include")
include_directories("${PROJECT_SOURCE_DIR}/tedcore/include")
include_directories("${PROJECT_SOURCE_DIR}/tedlang/include")

add_library(
	libtedtext SHARED
	"src/temp.cpp"
)

target_link_libraries(libtedtext PRIVATE libtedcore)
target_link_libraries(libtedtext PRIVATE libtedl)

# freetype (fonts library)
find_package(freetype CONFIG REQUIRED)
target_link_libraries(libtedtext PRIVATE freetype)

# Copying fonts
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tedtext/fonts")
merge_dir("${CMAKE_CURRENT_BINARY_DIR}/tedtext/fonts" "${PROJECT_SOURCE_DIR}/tedtext/fonts")
