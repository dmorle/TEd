#ifndef TE_TEDLANG_H
#define TE_TEDLANG_H

#ifdef _MSC_VER
//  Microsoft 
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
#	include <tedlang/core/eval.h>
#	include <tedlang/parse/parser.h>
#	include <tedlang/parse/lexer.h>
#endif

#endif
