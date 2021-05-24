
# tedlang

cmake_minimum_required (VERSION 3.8)

include_directories("include")

# libtedl

if (WIN32)
	enable_language(ASM_MASM)

	add_library(
		libtedl SHARED

		"src/core/obj.c"
		"src/core/eval.c"
		"src/core/scope.c"
		"src/parse/lexer.c"
		"src/parse/parser.c"
		"src/core/windows/import.c"
		"src/util/string.asm"

		"src/builtin/null.c"
		"src/builtin/bool.c"
		"src/builtin/char.c"
		"src/builtin/int.c"
		"src/builtin/float.c"
		"src/builtin/str.c"
		"src/builtin/arr.c"
		"src/builtin/fn.c"

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

else()
	message(FATAL_ERROR "tedlang currently only compiles for windows")
endif(WIN32)

# tedlang

add_executable(
	tedlang
	"src/main.c"
	"include/tedlang/tedl.h"
)
target_link_libraries(tedlang libtedl)

set_property(TARGET tedlang PROPERTY LINKER_LANGUAGE C)
set_property(TARGET tedlang PROPERTY C_STANDARD 11)
