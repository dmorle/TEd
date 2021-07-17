#include <io/io.h>

static te_obj_st print_fn;

te_scope_st* te_mod_init(te_scope_st* pscope)
{
	// initializing functions
	te_obj_new(&print_fn, &_te_print_ty);
	if (te_haserr())
		return NULL;

	// injecting functions into scope
	if (!te_scope_set(pscope, "print", &print_fn))
		return NULL;

	return pscope;
}
