#ifndef TEDUI_H
#define TEDUI_H

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


#ifndef TEDUI_API
#	ifdef TEDUI_SRC
#		define TEDUI_API extern "C" EXPORT
#	else
#		define TEDUI_API extern "C" IMPORT
#	endif
#endif

TEDUI_API void tedui_startup();
TEDUI_API void tedui_shutdown();

namespace tedui
{
	
}

#endif
