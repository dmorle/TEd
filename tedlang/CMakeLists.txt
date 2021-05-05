include_directories("include")

add_library(
	libtel SHARED

	"src/core/obj.c"
	"src/core/int.c"
	"src/core/arr.c"
	"src/core/str.c"

	"src/parse/lexer.c"
	"src/parse/parser.c"

	"src/interpreter/eval.c"

	"include/tedlang/tedl.h"
	"include/tedlang/core/obj.h"
	"include/tedlang/core/int.h"
	"include/tedlang/core/arr.h"
	"include/tedlang/core/str.h"

	"include/tedlang/parse/lexer.h"
	"include/tedlang/parse/parser.h"

	"include/tedlang/interpreter/eval.h"
)

add_executable(
	telang

	"src/main.c"

	"include/tedlang/tedl.h"
)
target_link_libraries(telang libtel)

set_property(TARGET telang PROPERTY C_STANDARD 11)
set_property(TARGET libtel PROPERTY C_STANDARD 11)
