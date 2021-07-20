#include <os/os.h>
#include <os/cptr.h>

#ifdef _MSC_VER
#include <windows.h>
#else
#error Unsupported OS
#endif

const te_type_st _te_load_proc_ty = {
	.name = "fn",
	.objsize = sizeof(te_obj_st),
	.ty_bool = te_load_proc_bool,
	.ty_call = te_load_proc_call
};

#define CHECK_TYPE_RET(ret) if (pself->ty != &_te_load_proc_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_TYPE CHECK_TYPE_RET(NULL)

bool te_load_proc_bool(te_obj_st * pself)
{
	CHECK_TYPE_RET(false);
	return true;
}

te_obj_st* te_load_proc_call(te_obj_st * pself, const te_fnargs_st * pargs)
{
	if (pargs->argc != 2)
		return te_seterr("load_proc expected 2 arguments, but recieved %zu", pargs->argc);
	if (pargs->ppargs[0]->ty != &_te_cptr_ty)
		return te_seterr("Invalild argument type in load_proc function, expected cptr type as first argument");
	if (pargs->ppargs[1]->ty != &_te_str_ty)
		return te_seterr("Invalild argument type in load_proc function, expected str type as second argument");

	te_cptr_st* pmod = (te_cptr_st*)pargs->ppargs[0];
	te_str_st* pproc_name = (te_str_st*)pargs->ppargs[1];
	te_cptr_st* pproc = (te_cptr_st*)te_cptr_new();
	if (te_haserr())
		return NULL;

#ifdef _MSC_VER
	FARPROC proc = GetProcAddress(pmod->ptr, pproc_name->val);
	if (!proc)
	{
		te_decref(pproc);
		return te_seterr("Unable to retrieve procedure '%s' from the given library", pproc_name->val);
	}

	pproc->ptr = proc;
#else
#error Unsupported OS
#endif

	return pproc;
}
