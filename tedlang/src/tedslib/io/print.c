#include <io/io.h>

const te_type_st _te_print_ty = {
	.name = "fn",
	.objsize = sizeof(te_obj_st),
	.ty_bool = te_print_bool,
	.ty_call = te_print_call
};

#define CHECK_TYPE_RET(ret) if (pself->ty != &_te_print_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_TYPE CHECK_TYPE_RET(NULL)

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
