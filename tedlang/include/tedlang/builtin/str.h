#ifndef TE_STR_H
#define TE_STR_H

#include <tedlang/tedl.h>
#include <tedlang/core/obj.h>

TEDLANG_API te_obj_st* te_str_new();
TEDLANG_API void te_str_del(te_obj_st* pself);

TEDLANG_API const char* te_str_str(te_obj_st* pself);
TEDLANG_API te_iterable_st* te_str_iter(te_obj_st* pself);
TEDLANG_API bool te_str_bool(te_obj_st* pself);
TEDLANG_API te_obj_st* te_str_idx(te_obj_st* pself, te_obj_st* pidx);

TEDLANG_API te_obj_st* te_str_iadd(te_obj_st* pself, te_obj_st* plval);
TEDLANG_API te_obj_st* te_str_add(te_obj_st* pself, te_obj_st* plval);
TEDLANG_API te_obj_st* te_str_eq(te_obj_st* pself, te_obj_st* plval);
TEDLANG_API te_obj_st* te_str_ne(te_obj_st* pself, te_obj_st* plval);

static te_type_st _te_str_ty = {
	.ty_new = te_str_new,
	.ty_del = te_str_del,
	.ty_str = te_str_str,
	.ty_iter = te_str_iter,
	.ty_bool = te_str_bool,
	.ty_idx = te_str_idx,
	.ty_iadd = te_str_iadd,
	.ty_add = te_str_add,
	.ty_eq = te_str_eq,
	.ty_ne = te_str_ne
};

typedef struct
{
	te_obj_st;
	char* val;
}
te_str_st;

#endif
