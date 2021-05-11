#define TEDLANG_SRC

#include <tedlang/core/obj.h>
#include <tedlang/builtin/bool.h>

te_type_st _te_bool_ty = {
	.name = "bool",
	.objsize = sizeof(te_bool_st),
	.ty_new = te_bool_new,
	.ty_del = te_bool_del,
	.ty_repr = te_bool_repr,
	.ty_bool = te_bool_bool
};
