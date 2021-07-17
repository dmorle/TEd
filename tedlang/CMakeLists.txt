
# tedlang

cmake_minimum_required (VERSION 3.8)

include_directories("include")

# libtedl

if (WIN32)
	enable_language(ASM_MASM)

	add_library(
		libtedl SHARED

		"src/tedlang/core/obj.c"
		"src/tedlang/core/eval.c"
		"src/tedlang/core/scope.c"
		"src/tedlang/parse/lexer.c"
		"src/tedlang/parse/parser.c"
		"src/tedlang/core/windows/import.c"
		"src/tedlang/util/string.asm"

		"src/tedlang/builtin/null.c"
		"src/tedlang/builtin/bool.c"
		"src/tedlang/builtin/char.c"
		"src/tedlang/builtin/int.c"
		"src/tedlang/builtin/float.c"
		"src/tedlang/builtin/str.c"
		"src/tedlang/builtin/arr.c"
		"src/tedlang/builtin/fn.c"

		"include/tedlang/tedl.h"
		"include/tedlang/core/obj.h"
		"include/tedlang/core/eval.h"
		"include/tedlang/core/scope.h"
		"include/tedlang/core/import.h"
		"include/tedlang/parse/lexer.h"
		"include/tedlang/parse/parser.h"
		"include/tedlang/util/string.h"

		"include/tedlang/builtin/null.h"
		"include/tedlang/builtin/bool.h"
		"include/tedlang/builtin/char.h"
		"include/tedlang/builtin/int.h"
		"include/tedlang/builtin/float.h"
		"include/tedlang/builtin/str.h"
		"include/tedlang/builtin/arr.h"
		"include/tedlang/builtin/fn.h"
	)

	target_link_libraries(libtedl user32.lib)
	target_link_libraries(libtedl kernel32.lib)

	set_property(TARGET libtedl PROPERTY C_STANDARD 11)

	# tedlang standard library

	add_library(
		io SHARED

		"src/tedslib/io/io.c"
		"src/tedslib/io/print.c"
		
		"include/tedslib/io/io.h"
	)

	target_include_directories(io PRIVATE "include/tedslib")
	target_link_libraries(io libtedl)

	set_target_properties(io PROPERTIES
		SUFFIX ".tedext"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tedlang/slib"
		C_STANDARD 11
	)

	add_library(
		os SHARED

		"src/tedslib/os/os.c"
		"src/tedslib/os/cptr.c"

		"src/tedslib/os/open_lib.c"
		"src/tedslib/os/close_lib.c"
		"src/tedslib/os/load_proc.c"
		
		"include/tedslib/os/os.h"
		"include/tedslib/os/cptr.h"
	)

	target_include_directories(os PRIVATE "include/tedslib")
	target_link_libraries(os libtedl)
	target_link_libraries(os user32.lib)
	target_link_libraries(os kernel32.lib)

	set_target_properties(os PROPERTIES
		SUFFIX ".tedext"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tedlang/slib"
		C_STANDARD 11
	)

else()
	message(FATAL_ERROR "tedlang currently only compiles for windows")
endif(WIN32)

# custom target for the entire standard library
add_custom_target(tedl_slib DEPENDS io os)

# tedlang

add_executable(
	tedlang
	"src/tedlang/main.c"
	"include/tedlang/tedl.h"
)
target_link_libraries(tedlang libtedl)
add_dependencies(tedlang tedl_slib)

set_property(TARGET tedlang PROPERTY LINKER_LANGUAGE C)
set_property(TARGET tedlang PROPERTY C_STANDARD 11)
