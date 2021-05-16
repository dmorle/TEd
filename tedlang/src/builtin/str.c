#define TEDLANG_SRC

#include <tedlang/core/obj.h>
#include <tedlang/builtin/str.h>

te_type_st _te_str_ty = {
	.name = "str",
	.objsize = sizeof(te_str_st),
	.ty_new = te_str_new,
	.ty_del = te_str_del,
	.ty_repr = te_str_repr,
	.ty_start = te_str_start,
	.ty_next = te_str_next,
	.ty_bool = te_str_bool,
	.ty_idx = te_str_idx,
	.ty_iadd = te_str_iadd,
	.ty_add = te_str_add,
	.ty_eq = te_str_eq,
	.ty_ne = te_str_ne
};
