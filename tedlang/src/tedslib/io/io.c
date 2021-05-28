#include <io/io.h>

te_print_st print_fn;

te_scope_st* te_mod_init(te_scope_st* pscope)
{
	te_obj_new(&print_fn, &_te_print_ty);
	if (te_haserr())
		return NULL;

	return te_scope_set(pscope, "print", &print_fn);
}
