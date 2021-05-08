add_library(
	libtest SHARED
	"shared/shared.c"
	"shared/shared.h"
)
target_include_directories(libtest PUBLIC "shared")

add_executable(
	main
	"main/main.c"
)
target_link_libraries(main libtest)
target_include_directories(main PUBLIC "shared")

set_property(TARGET libtest PROPERTY C_STANDARD 11)
