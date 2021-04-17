include_directories("inc")

add_library(
	libtish SHARED

	"src/core/obj.c"
	"src/core/int.c"
	"src/core/arr.c"
	"src/core/str.c"
	"src/parse/lexer.c"
	"src/parse/parser.c"

	"inc/tesh/tishapi.h"
	"inc/tesh/core/obj.h"
	"inc/tesh/core/int.h"
	"inc/tesh/core/arr.h"
	"inc/tesh/core/str.h"
	"inc/tesh/parse/lexer.h"
	"inc/tesh/parse/parser.h"
)

add_executable(
	tish

	"src/main.c"

	"inc/tesh/tishapi.h"
)
target_link_libraries(tish libtish)

set_property(TARGET tish PROPERTY C_STANDARD 11)
set_property(TARGET libtish PROPERTY C_STANDARD 11)
