#ifndef TE_EVAL_H
#define TE_EVAL_H

#include <tedlang/tedl.h>

#include <tedlang/core/obj.h>
#include <tedlang/core/scope.h>
#include <tedlang/parse/parser.h>

TEDLANG_API int te_eval(const te_ast_st* past);

#endif
