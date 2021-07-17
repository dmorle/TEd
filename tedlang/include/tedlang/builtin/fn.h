#ifndef TEDLANG_FN_H
#define TEDLANG_FN_H

#include <tedlang/tedl.h>
#include <tedlang/core/obj.h>
#include <tedlang/parse/parser.h>

TEDLANG_API te_obj_st* te_fn_new();
TEDLANG_API void te_fn_del(te_obj_st* pself);

TEDLANG_API const char* te_fn_repr(te_obj_st* pself);
TEDLANG_API bool te_fn_bool(te_obj_st* pself);
TEDLANG_API te_obj_st* te_fn_call(te_obj_st* pself, const te_fnargs_st* pargs);

TEDLANG_API const extern te_type_st _te_fn_ty;

typedef struct
{
	te_obj_st super;
	te_scope_st* pscope;  // reference to the scope in which the function was defined
	te_ast_st* pbody;
	size_t argc;
	char** ppargv;
}
te_fn_st;

#endif
