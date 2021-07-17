#include <os/os.h>
#include <os/cptr.h>

#include <string.h>

#ifdef _MSC_VER
#include <windows.h>
#else
#error Unsupported OS
#endif

const te_type_st _te_open_lib_ty = {
	.name = "fn",
	.objsize = sizeof(te_obj_st),
	.ty_bool = te_open_lib_bool,
	.ty_call = te_open_lib_call
};

#define CHECK_TYPE_RET(ret) if (pself->ty != &_te_open_lib_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_TYPE CHECK_TYPE_RET(NULL)

bool te_open_lib_bool(te_obj_st* pself)
{
	CHECK_TYPE_RET(false);
	return true;
}

te_obj_st* te_open_lib_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	CHECK_TYPE;

	if (pargs->argc != 1)
		return te_seterr("open_lib expected 1 argument, but recieved %zu", pargs->argc);
	if (strcmp(pargs->ppargs[0]->ty->name, "str"))
		return te_seterr("Invalild argument type to open_lib function, expected str");

	te_str_st* pstr = pargs->ppargs[0];
	te_cptr_st* nptr = (te_cptr_st*)te_cptr_new();
	if (te_haserr())
		return NULL;

#ifdef _MSC_VER
	HMODULE hmod = LoadLibrary(pstr->val);
	if (!hmod)
		return te_seterr("Failed to load library: %s", pstr->val);

	nptr->ptr = hmod;
#else
#error Unsupported OS
#endif

	return nptr;
}
