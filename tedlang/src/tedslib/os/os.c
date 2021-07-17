#include <os/os.h>

static te_obj_st open_lib_fn;
static te_obj_st close_lib_fn;
static te_obj_st load_proc_fn;

te_scope_st* te_mod_init(te_scope_st* pscope)
{
	// initializing functions
	te_obj_new(&open_lib_fn, &_te_open_lib_ty);
	if (te_haserr())
		return NULL;
	te_obj_new(&close_lib_fn, &_te_close_lib_ty);
	if (te_haserr())
		return NULL;
	te_obj_new(&load_proc_fn, &_te_load_proc_ty);
	if (te_haserr())
		return NULL;

	// injecting functions into scope
	if (!te_scope_set(pscope, "open_lib", &open_lib_fn))
		return NULL;
	if (!te_scope_set(pscope, "close_lib", &close_lib_fn))
		return NULL;
	if (!te_scope_set(pscope, "load_proc", &load_proc_fn))
		return NULL;

	return pscope;
}
