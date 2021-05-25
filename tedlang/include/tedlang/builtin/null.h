#ifndef TEDLANG_NULL_H
#define TEDLANG_NULL_H

#include <stdbool.h>

#include <tedlang/tedl.h>
#include <tedlang/core/obj.h>

TEDLANG_API te_obj_st* te_null_new();
TEDLANG_API void te_null_del(te_obj_st* pself);

TEDLANG_API const char* te_null_repr(te_obj_st* pself);

TEDLANG_API te_obj_st* te_null_eq(te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_null_ne(te_obj_st* pself, te_obj_st* prval);

extern te_type_st _te_null_ty;

typedef struct
{
	te_obj_st super;
}
te_null_st;

#endif
