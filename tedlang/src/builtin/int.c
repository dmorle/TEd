#define TEDLANG_SRC

#include <tedlang/core/obj.h>
#include <tedlang/builtin/int.h>

te_type_st _te_int_ty = {
	.ty_new = te_int_new,
	.ty_del = te_int_del,
	.ty_repr = te_int_repr,
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
