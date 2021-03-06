#ifndef TEDLANG_IMPORT_H
#define TEDLANG_IMPORT_H

#include <tedlang/core/scope.h>
#include <tedlang/parse/parser.h>

// Called by te_init()
void te_init_imports();
void te_free_imports();
// returns impdir on success, NULL on failure
TEDLANG_API char* te_add_impdir(char* impdir);
// TODO: add a mechanism for adding/removing/listing import directories

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

// loads a tedlang script from its path
TEDLANG_API te_module_st* module_load_script(te_module_st* pmodule, const char* pth);

// loads a tedlang extension from its path
TEDLANG_API te_module_st* module_load_bin(te_module_st* pmodule, const char* pth);

// loads a tedlang module from a file path
TEDLANG_API te_module_st* module_load_pth(te_module_st* pmodule, const char* pth);

// finds and opens a tedlang script or extension
// for use by te_eval_imp
TEDLANG_API te_module_st* module_load(te_module_st* pmodule, const te_ast_imp_st* pimp);

// imports an opened module into a given scope
TEDLANG_API te_scope_st* module_import(te_scope_st* pscope, const te_module_st* pmodule);

// releases the resources allocated by module_load
TEDLANG_API void module_close(te_module_st* pmodule);

#endif
