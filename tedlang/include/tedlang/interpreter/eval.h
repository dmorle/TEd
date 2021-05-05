#ifndef TE_EVAL_H
#define TE_EVAL_H

#include <tedlang/tedl.h>

#include <tedlang/parse/parser.h>

TE_API int te_eval(const te_ast_st* past, te_obj_st** ppobj);

#endif
