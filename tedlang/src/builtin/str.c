#define TEDLANG_SRC

#include <tedlang/core/obj.h>
#include <tedlang/builtin/str.h>

te_type_st _te_str_ty = {
	.ty_new = te_str_new,
	.ty_del = te_str_del,
	.ty_repr = te_str_repr,
	.ty_iter = te_str_iter,
	.ty_bool = te_str_bool,
	.ty_idx = te_str_idx,
	.ty_iadd = te_str_iadd,
	.ty_add = te_str_add,
	.ty_eq = te_str_eq,
	.ty_ne = te_str_ne
};
