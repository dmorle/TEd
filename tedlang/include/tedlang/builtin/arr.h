#ifndef TEDLANG_ARR_H
#define TEDLANG_ARR_H

#include <stdbool.h>

#include <tedlang/tedl.h>
#include <tedlang/core/obj.h>

TEDLANG_API te_obj_st* te_arr_new();
TEDLANG_API void te_arr_del(te_obj_st* pself);

TEDLANG_API bool te_arr_bool(te_obj_st* pself);
TEDLANG_API const char* te_arr_repr(te_obj_st* pself);
TEDLANG_API te_obj_st* te_arr_start(te_obj_st* pself);
TEDLANG_API te_obj_st* te_arr_next(te_obj_st* pself);

TEDLANG_API te_obj_st* te_arr_iadd(te_obj_st** ppself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_arr_add(te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_arr_eq(te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_arr_ne(te_obj_st* pself, te_obj_st* prval);

extern te_type_st _te_arr_ty;

typedef struct
{
	te_obj_st super;
	size_t length;
	size_t _mem_sz;
	te_obj_st** ppelems;
}
te_arr_st;

#endif
