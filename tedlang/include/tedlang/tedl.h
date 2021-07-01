#ifndef TEDLANG_H
#define TEDLANG_H

#ifdef _MSC_VER
//  Microsoft
#	pragma warning( disable : 4133 4244 4267 )
#	define EXPORT __declspec(dllexport)
#	define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
//  GCC
#	define EXPORT __attribute__((visibility("default")))
#	define IMPORT
#else
//  do nothing and hope for the best?
#	define EXPORT
#	define IMPORT
#	pragma warning Unknown dynamic link import/export semantics.
#endif


#ifndef TEDLANG_API
#	ifdef TEDLANG_SRC
#		define TEDLANG_API EXPORT
#	else
#		define TEDLANG_API IMPORT
#	endif
#endif

#ifndef TEDLANG_SRC
#	ifdef __cplusplus
#		define TEDLANG_MODINIT extern "C" EXPORT
extern "C"
{
#	else
#		define TEDLANG_MODINIT EXPORT
#	endif

#	include <tedlang/core/eval.h>
#	include <tedlang/core/obj.h>
#	include <tedlang/core/scope.h>
#	include <tedlang/core/import.h>
#	include <tedlang/parse/parser.h>
#	include <tedlang/parse/lexer.h>

#	include <tedlang/builtin/null.h>
#	include <tedlang/builtin/bool.h>
#	include <tedlang/builtin/char.h>
#	include <tedlang/builtin/int.h>
#	include <tedlang/builtin/float.h>
#	include <tedlang/builtin/str.h>
#	include <tedlang/builtin/arr.h>
#	include <tedlang/builtin/fn.h>

#	ifdef __cplusplus
}
#	endif
#else
#	define DEFAULT_SCOPESZ 64
#	define DEFAULT_SCOPELF 0.75
#endif

#endif
