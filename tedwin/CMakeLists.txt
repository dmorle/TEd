
# tedwin

cmake_minimum_required (VERSION 3.8)

include_directories("include")
include_directories("include")

add_executable(
	ted WIN32
	"src/main.c"
	"src/ted.c"

	"include/ted/ted.h"
)
