
# tedcore

cmake_minimum_required (VERSION 3.8)

include_directories("include")
include_directories("${PROJECT_SOURCE_DIR}/tedlang/include")

add_library(
	libtedcore SHARED
	"src/temp.cpp"
)

target_link_libraries(libtedcore PRIVATE libtedlang)
