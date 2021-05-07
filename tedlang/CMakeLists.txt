include_directories("include")

add_library(
	libtedl SHARED

	"src/core/obj.c"
	"src/core/eval.c"
	"src/core/scope.c"
	"src/parse/lexer.c"
	"src/parse/parser.c"

	"include/tedlang/tedl.h"
	"include/tedlang/core/obj.h"
	"include/tedlang/core/eval.h"
	"include/tedlang/core/scope.h"
	"include/tedlang/parse/lexer.h"
	"include/tedlang/parse/parser.h"
)

add_executable(
	tedlang
	"src/main.c"
	"include/tedlang/tedl.h"
)
target_link_libraries(tedlang libtedl)

set_property(TARGET tedlang PROPERTY C_STANDARD 11)
set_property(TARGET libtedl PROPERTY C_STANDARD 11)
