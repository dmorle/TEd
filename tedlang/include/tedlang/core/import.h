#ifndef TEDLANG_IMPORT_H
#define TEDLANG_IMPORT_H

#include <tedlang/core/scope.h>
#include <tedlang/parse/parser.h>

#define MAX_IMPDIRS 1024

extern char* pimpdirs[MAX_IMPDIRS];
extern size_t nimpdirs;

typedef enum
{
	TE_MODULE_NONE = 0, // uninitialized
	TE_MODULE_INV,      // invalid module
	TE_MODULE_SCRIPT,   // *.ted file
	TE_MODULE_BIN       // shared library with a tedlang interface
}
te_module_et;

typedef te_scope_st* (*te_loadmod_t)(te_scope_st*);

typedef struct
{
	te_module_et ty;
	union {
		te_ast_st* past;
		te_loadmod_t loadmod;
	};
}
te_module_st;

// finds and opens a tedlang module or extension
te_module_st* open_module_ast(te_module_st* pmodule, const te_ast_imp_st* pimp);
te_module_st* open_module_raw(te_module_st* pmodule, const char* modpth);

// imports an opened module into a given scope
te_scope_st* import_module(te_scope_st* pscope, const te_module_st* pmodule);

// releases the resources allocated by open_module
void close_module(te_module_st* pmodule);

#endif
