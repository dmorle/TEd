include_directories("include")

add_library(
	libtesh SHARED

	"src/core/obj.c"
	"src/core/int.c"
	"src/core/arr.c"
	"src/core/str.c"

	"src/parse/lexer.c"
	"src/parse/parser.c"

	"src/interpreter/eval.c"

	"include/tesh/teshapi.h"
	"include/tesh/core/obj.h"
	"include/tesh/core/int.h"
	"include/tesh/core/arr.h"
	"include/tesh/core/str.h"

	"include/tesh/parse/lexer.h"
	"include/tesh/parse/parser.h"

	"include/tesh/interpreter/eval.h"
)

add_executable(
	tesh

	"src/main.c"

	"include/tesh/teshapi.h"
)
target_link_libraries(tesh libtesh)

set_property(TARGET tesh PROPERTY C_STANDARD 11)
set_property(TARGET libtesh PROPERTY C_STANDARD 11)
