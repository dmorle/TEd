#define TEDLANG_SRC

#include <stdlib.h>
#include <stdbool.h>

#include <tedlang/core/obj.h>
#include <tedlang/core/eval.h>

void te_del(te_obj_st* pself)
{
	if (pself->ty->ty_del)
		pself->ty->ty_del(pself);
	else
		free(pself);
}

te_obj_st* te_cpy(te_obj_st* pself)
{
	if (pself->ty->ty_cpy)
		return pself->ty->ty_cpy(pself);
	else
	{
		// TODO: Implemenation
		// te_seterr("Out of memory\n");
	}
}

bool te_bool(te_obj_st* pself)
{
}

int64_t te_int(te_obj_st* pself)
{
}

const char* te_repr(te_obj_st* pself)
{
}

te_obj_st* te_call(te_obj_st* pself, te_fnargs_st args)
{
}

te_iterable_st* te_iter(te_obj_st* pself)
{
}

te_obj_st* te_not(te_obj_st* pself)
{
}

te_obj_st** te_idx(te_obj_st* pself, te_obj_st* prval)
{
}

te_obj_st* te_assign(te_obj_st** ppself, te_obj_st* prval)
{
}

te_obj_st* te_iadd(te_obj_st** ppself, te_obj_st* prval)
{
}

te_obj_st* te_isub(te_obj_st** ppself, te_obj_st* prval)
{
}

te_obj_st* te_imul(te_obj_st** ppself, te_obj_st* prval)
{
}

te_obj_st* te_idiv(te_obj_st** ppself, te_obj_st* prval)
{
}

te_obj_st* te_imod(te_obj_st** ppself, te_obj_st* prval)
{
}

te_obj_st* te_iexp(te_obj_st** ppself, te_obj_st* prval)
{
}

te_obj_st* te_add(te_obj_st* plval, te_obj_st* prval)
{
}

te_obj_st* te_sub(te_obj_st* plval, te_obj_st* prval)
{
}

te_obj_st* te_mul(te_obj_st* plval, te_obj_st* prval)
{
}

te_obj_st* te_div(te_obj_st* plval, te_obj_st* prval)
{
}

te_obj_st* te_mod(te_obj_st* plval, te_obj_st* prval)
{
}

te_obj_st* te_exp(te_obj_st* plval, te_obj_st* prval)
{
}

te_obj_st* te_and(te_obj_st* plval, te_obj_st* prval)
{
}

te_obj_st* te_or(te_obj_st* plval, te_obj_st* prval)
{
}

te_obj_st* te_eq(te_obj_st* plval, te_obj_st* prval)
{
}

te_obj_st* te_ne(te_obj_st* plval, te_obj_st* prval)
{
}

te_obj_st* te_lt(te_obj_st* plval, te_obj_st* prval)
{
}

te_obj_st* te_gt(te_obj_st* plval, te_obj_st* prval)
{
}

te_obj_st* te_le(te_obj_st* plval, te_obj_st* prval)
{
}

te_obj_st* te_ge(te_obj_st* plval, te_obj_st* prval)
{
}


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
