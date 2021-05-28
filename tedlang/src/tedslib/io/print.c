#include <io/io.h>

te_type_st _te_print_ty = {
	.name = "print_fn",
	.objsize = sizeof(te_print_st),
	.ty_new = te_print_new,
	.ty_bool = te_print_bool,
	.ty_call = te_print_call
};

#define self (*(te_fn_st*)pself)
#define CHECK_TYPE_RET(ret) if (pself->ty != &_te_print_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_TYPE CHECK_TYPE_RET(NULL)

te_obj_st* te_print_new()
{
	return te_seterr("Invalid operation on print function");
}

bool te_print_bool(te_obj_st* pself)
{
	CHECK_TYPE_RET(false);
	return true;
}

te_obj_st* te_print_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	for (size_t i = 0; i < pargs->argc; i++)
	{
		const char* repr = te_repr(pargs->ppargs[i]);
		if (te_haserr())
			return NULL;
		printf(repr);
	}
	fflush(stdout);
	return NULL;
}
