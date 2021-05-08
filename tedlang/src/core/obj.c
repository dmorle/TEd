#define TEDLANG_SRC

#include <stdlib.h>
#include <stdbool.h>

#include <tedlang/core/obj.h>


void te_incref(te_obj_st* pobj)
{
	pobj->n_ref++;
}

void te_decref(te_obj_st* pobj)
{
	pobj->n_ref--;
	if (pobj->n_ref)
		return;

	pobj->is_valid = false;
	pobj->ty->ty_del(pobj);
	free(pobj);
}
