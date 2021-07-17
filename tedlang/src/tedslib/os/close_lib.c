#include <os/os.h>
#include <os/cptr.h>

#include <string.h>

#ifdef _MSC_VER
#include <windows.h>
#else
#error Unsupported OS
#endif

const te_type_st _te_close_lib_ty = {
	.name = "fn",
	.objsize = sizeof(te_obj_st),
	.ty_bool = te_close_lib_bool,
	.ty_call = te_close_lib_call
};

#define CHECK_TYPE_RET(ret) if (pself->ty != &_te_close_lib_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_TYPE CHECK_TYPE_RET(NULL)

bool te_close_lib_bool(te_obj_st * pself)
{
	CHECK_TYPE_RET(false);
	return true;
}

te_obj_st* te_close_lib_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	if (pargs->argc != 1)
		return te_seterr("open_lib expected 1 argument, but recieved %zu", pargs->argc);
	if (pargs->ppargs[0]->ty != &_te_cptr_ty)
		return te_seterr("Invalild argument type to close_lib function, expected exact cptr type");

#ifdef _MSC_VER
	HMODULE hmod = ((te_cptr_st*)pargs->ppargs[0])->ptr;
	if (!hmod)
		return te_seterr("Invalid module handle: NULL");
	if (FreeLibrary(hmod) == FALSE)
		return te_seterr("Failed to close library");
	return te_null_new();
#else
#error Unsupported OS
#endif
}
