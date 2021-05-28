#ifndef TE_EVAL_H
#define TE_EVAL_H

#include <tedlang/tedl.h>

#include <tedlang/core/obj.h>
#include <tedlang/core/scope.h>
#include <tedlang/parse/parser.h>

extern te_scope_st global_scope;

TEDLANG_API te_scope_st* te_init(); // initializes the interpreter
TEDLANG_API void te_shutdown();

TEDLANG_API bool  te_haserr();
TEDLANG_API void* te_seterr(const char* err, ...);  // always returns NULL

TEDLANG_API te_obj_st** te_lval(te_scope_st* pscope, const te_ast_st* past);
TEDLANG_API te_obj_st* te_eval(te_scope_st* pscope, te_ast_st* past);

#endif
