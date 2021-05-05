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

	"include/telang/telapi.h"
	"include/telang/core/obj.h"
	"include/telang/core/int.h"
	"include/telang/core/arr.h"
	"include/telang/core/str.h"

	"include/telang/parse/lexer.h"
	"include/telang/parse/parser.h"

	"include/telang/interpreter/eval.h"
)

add_executable(
	telang

	"src/main.c"

	"include/telang/telapi.h"
)
target_link_libraries(telang libtel)

set_property(TARGET telang PROPERTY C_STANDARD 11)
set_property(TARGET libtel PROPERTY C_STANDARD 11)
