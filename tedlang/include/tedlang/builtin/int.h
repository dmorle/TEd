#ifndef TE_INT_H
#define TE_INT_H

#include <stdbool.h>

#include <tedlang/tedl.h>
#include <tedlang/core/obj.h>

TE_API te_obj_st* te_int_new();
TE_API void te_int_del(te_obj_st* pself);

TE_API bool te_int_bool(te_obj_st* pself);
TE_API int64_t te_int_int(te_obj_st* pself);
TE_API const char* te_int_str(te_obj_st* pself);
TE_API te_iterable_st* te_int_iter(te_obj_st* pself);

TE_API te_obj_st* te_int_iadd(te_obj_st* pself, te_obj_st* plval);
TE_API te_obj_st* te_int_isub(te_obj_st* pself, te_obj_st* plval);
TE_API te_obj_st* te_int_imul(te_obj_st* pself, te_obj_st* plval);
TE_API te_obj_st* te_int_idiv(te_obj_st* pself, te_obj_st* plval);
TE_API te_obj_st* te_int_imod(te_obj_st* pself, te_obj_st* plval);
TE_API te_obj_st* te_int_add(te_obj_st* pself, te_obj_st* plval);
TE_API te_obj_st* te_int_sub(te_obj_st* pself, te_obj_st* plval);
TE_API te_obj_st* te_int_mul(te_obj_st* pself, te_obj_st* plval);
TE_API te_obj_st* te_int_div(te_obj_st* pself, te_obj_st* plval);
TE_API te_obj_st* te_int_mod(te_obj_st* pself, te_obj_st* plval);
TE_API te_obj_st* te_int_eq(te_obj_st* pself, te_obj_st* plval);
TE_API te_obj_st* te_int_ne(te_obj_st* pself, te_obj_st* plval);
TE_API te_obj_st* te_int_lt(te_obj_st* pself, te_obj_st* plval);
TE_API te_obj_st* te_int_gt(te_obj_st* pself, te_obj_st* plval);
TE_API te_obj_st* te_int_le(te_obj_st* pself, te_obj_st* plval);
TE_API te_obj_st* te_int_ge(te_obj_st* pself, te_obj_st* plval);

static te_type_st _te_int_ty = {
	.ty_new = te_int_new,
	.ty_del = te_int_del,
	.ty_str = te_int_str,
	.ty_bool = te_int_bool,
	.ty_bool = te_int_iter,
	.ty_iadd = te_int_iadd,
	.ty_isub = te_int_isub,
	.ty_imul = te_int_imul,
	.ty_idiv = te_int_idiv,
	.ty_imod = te_int_imod,
	.ty_add = te_int_add,
	.ty_sub = te_int_sub,
	.ty_mul = te_int_mul,
	.ty_div = te_int_div,
	.ty_mod = te_int_mod,
	.ty_eq = te_int_eq,
	.ty_ne = te_int_ne,
	.ty_lt = te_int_lt,
	.ty_gt = te_int_gt,
	.ty_le = te_int_le,
	.ty_ge = te_int_ge
};

typedef struct
{
	te_obj_st;
	int64_t val;
}
te_int_st;

#endif
