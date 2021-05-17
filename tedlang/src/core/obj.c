#define TEDLANG_SRC

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <tedlang/core/obj.h>
#include <tedlang/core/eval.h>

#include <tedlang/builtin/bool.h>

void te_del(te_obj_st* pself)
{
	if (pself->ty->ty_del)
	{
		pself->ty->ty_del(pself);
		free(pself);
	}
	else
		free(pself);
}

te_obj_st* te_cpy(te_obj_st* pself)
{
	if (pself->ty->ty_cpy)
		return pself->ty->ty_cpy(pself);

	// allocate a new object with the same size, and does a shallow copy
	void* npobj = malloc(pself->ty->objsize);
	if (!npobj)
		return te_seterr("Out of memory");
	return memcpy(npobj, pself, pself->ty->objsize);
}

bool te_bool(te_obj_st* pself)
{
	if (pself->ty->ty_bool)
		return pself->ty->ty_bool(pself);

	return false;
}

int64_t te_int(te_obj_st* pself)
{
	if (pself->ty->ty_int)
		return pself->ty->ty_int(pself);

	te_seterr("Unable to cast type %s to int", pself->ty->name);
	return -1;
}

const char* te_repr(te_obj_st* pself)
{
	if (pself->ty->ty_repr)
		return pself->ty->ty_repr(pself);

	return pself->ty->name;
}

te_obj_st* te_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	if (pself->ty->ty_call)
		return pself->ty->ty_call(pself, pargs);

	return te_seterr("Unable to call object with type %s", pself->ty->name);
}

te_obj_st* te_start(te_obj_st* pself)
{
	if (pself->ty->ty_start)
		return pself->ty->ty_start(pself);

	return te_seterr("Unable to create an iteration context for an object with type %s", pself->ty->name);
}

te_obj_st* te_next(te_obj_st* pself, te_obj_st* pctx)
{
	if (pself->ty->ty_next)
		return pself->ty->ty_next(pself, pctx);

	return te_seterr("Unable to iterate through an object with type %s", pself->ty->name);
}

te_obj_st* te_not(te_obj_st* pself)
{
	if (pself->ty->ty_not)
		return pself->ty->ty_not(pself);

	te_bool_st* pbool = te_bool_new();
	if (!pbool)
		return NULL;
	pbool->val = te_bool(pself);
	return pbool;
}

te_obj_st** te_idx(te_obj_st* pself, te_obj_st* prval)
{
	if (pself->ty->ty_idx)
		return pself->ty->ty_idx(pself, prval);

	return te_seterr("Unable to index an object with type %s", pself->ty->name);
}

te_obj_st* te_assign(te_obj_st** ppself, te_obj_st* prval)
{
	if (*ppself && (*ppself)->ty->ty_assign)
		return (*ppself)->ty->ty_assign(ppself, prval);

	te_incref(prval);
	te_decref(*ppself);
	*ppself = prval;
	return *ppself;
}

te_obj_st* te_iadd(te_obj_st** ppself, te_obj_st* prval)
{
	if ((*ppself)->ty->ty_iadd)
		return (*ppself)->ty->ty_iadd(ppself, prval);

	te_obj_st* ptmp = te_add(*ppself, prval);
	if (!ptmp)
		return NULL;
	te_obj_st* pret = te_assign(ppself, ptmp);
	te_decref(ptmp);
	return pret;  // NULL is returned if te_assign failed
}

te_obj_st* te_isub(te_obj_st** ppself, te_obj_st* prval)
{
	if ((*ppself)->ty->ty_isub)
		return (*ppself)->ty->ty_isub(ppself, prval);

	te_obj_st* ptmp = te_sub(*ppself, prval);
	if (!ptmp)
		return NULL;
	te_obj_st* pret = te_assign(ppself, ptmp);
	te_decref(ptmp);
	return pret;
}

te_obj_st* te_imul(te_obj_st** ppself, te_obj_st* prval)
{
	if ((*ppself)->ty->ty_imul)
		return (*ppself)->ty->ty_imul(ppself, prval);

	te_obj_st* ptmp = te_mul(*ppself, prval);
	te_obj_st* pret = te_assign(ppself, ptmp);
	te_decref(ptmp);
	return pret;
}

te_obj_st* te_idiv(te_obj_st** ppself, te_obj_st* prval)
{
	if ((*ppself)->ty->ty_idiv)
		return (*ppself)->ty->ty_idiv(ppself, prval);

	te_obj_st* ptmp = te_div(*ppself, prval);
	te_obj_st* pret = te_assign(ppself, ptmp);
	te_decref(ptmp);
	return pret;
}

te_obj_st* te_imod(te_obj_st** ppself, te_obj_st* prval)
{
	if ((*ppself)->ty->ty_imod)
		return (*ppself)->ty->ty_imod(ppself, prval);

	te_obj_st* ptmp = te_mod(*ppself, prval);
	if (!ptmp)
		return NULL;
	te_obj_st* pret = te_assign(ppself, ptmp);
	te_decref(ptmp);
	return pret;
}

te_obj_st* te_iexp(te_obj_st** ppself, te_obj_st* prval)
{
	if ((*ppself)->ty->ty_iexp)
		return (*ppself)->ty->ty_iexp(ppself, prval);

	te_obj_st* ptmp = te_exp(*ppself, prval);
	if (!ptmp)
		return NULL;
	te_obj_st* pret = te_assign(ppself, ptmp);
	te_decref(ptmp);
	return pret;
}

te_obj_st* te_add(te_obj_st* plval, te_obj_st* prval)
{
	if (plval->ty->ty_add)
		return plval->ty->ty_add(plval, prval);

	return te_seterr("Unable to add objects with type %s", plval->ty->name);
}

te_obj_st* te_sub(te_obj_st* plval, te_obj_st* prval)
{
	if (plval->ty->ty_sub)
		return plval->ty->ty_sub(plval, prval);

	return te_seterr("Unable to subtract objects with type %s", plval->ty->name);
}

te_obj_st* te_mul(te_obj_st* plval, te_obj_st* prval)
{
	if (plval->ty->ty_mul)
		return plval->ty->ty_mul(plval, prval);

	return te_seterr("Unable to multiply objects with type %s", plval->ty->name);
}

te_obj_st* te_div(te_obj_st* plval, te_obj_st* prval)
{
	if (plval->ty->ty_div)
		return plval->ty->ty_div(plval, prval);

	return te_seterr("Unable to divide objects with type %s", plval->ty->name);
}

te_obj_st* te_mod(te_obj_st* plval, te_obj_st* prval)
{
	if (plval->ty->ty_mod)
		return plval->ty->ty_mod(plval, prval);

	return te_seterr("Unable to modulo objects with type %s", plval->ty->name);
}

te_obj_st* te_exp(te_obj_st* plval, te_obj_st* prval)
{
	if (plval->ty->ty_exp)
		return plval->ty->ty_exp(plval, prval);

	return te_seterr("Unable to exponentiate objects with type %s", plval->ty->name);
}

te_obj_st* te_and(te_obj_st* plval, te_obj_st* prval)
{
	if (plval->ty->ty_and)
		return plval->ty->ty_and(plval, prval);
	
	te_bool_st* pbool = _te_bool_ty.ty_new();
	if (!pbool)
		return NULL;
	pbool->val = te_bool(plval) && te_bool(prval);
	return pbool;
}

te_obj_st* te_or(te_obj_st* plval, te_obj_st* prval)
{
	if (plval->ty->ty_or)
		return plval->ty->ty_or(plval, prval);

	te_bool_st* pbool = _te_bool_ty.ty_new();
	if (!pbool)
		return NULL;
	pbool->val = te_bool(plval) || te_bool(prval);
	return pbool;
}

te_obj_st* te_eq(te_obj_st* plval, te_obj_st* prval)
{
	if (plval->ty->ty_eq)
		return plval->ty->ty_eq(plval, prval);
	if (plval->ty->ty_ne)
	{
		te_obj_st* pne = plval->ty->ty_ne(plval, prval);
		if (!pne)
			return NULL;
		te_obj_st* pret = te_not(pne);
		te_decref(pne);
		return pret;
	}

	return te_seterr("Unable to compare objects with type %s", plval->ty->name);
}

te_obj_st* te_ne(te_obj_st* plval, te_obj_st* prval)
{
	if (plval->ty->ty_ne)
		return plval->ty->ty_ne(plval, prval);
	if (plval->ty->ty_eq)
	{
		te_obj_st* peq = plval->ty->ty_eq(plval, prval);
		if (!peq)
			return NULL;
		te_obj_st* pret = te_not(peq);
		te_decref(peq);
		return pret;
	}

	return te_seterr("Unable to compare objects with type %s", plval->ty->name);
}

te_obj_st* te_lt(te_obj_st* plval, te_obj_st* prval)
{
	if (plval->ty->ty_lt)
		return plval->ty->ty_lt(plval, prval);

	return te_seterr("Unable to compare objects with type %s", plval->ty->name);
}

te_obj_st* te_gt(te_obj_st* plval, te_obj_st* prval)
{
	if (plval->ty->ty_gt)
		return plval->ty->ty_gt(plval, prval);

	return te_seterr("Unable to compare objects with type %s", plval->ty->name);
}

te_obj_st* te_le(te_obj_st* plval, te_obj_st* prval)
{
	if (plval->ty->ty_le)
		return plval->ty->ty_le(plval, prval);

	return te_seterr("Unable to compare objects with type %s", plval->ty->name);
}

te_obj_st* te_ge(te_obj_st* plval, te_obj_st* prval)
{
	if (plval->ty->ty_ge)
		return plval->ty->ty_ge(plval, prval);

	return te_seterr("Unable to compare objects with type %s", plval->ty->name);
}


void te_incref(te_obj_st* pobj)
{
	if (!pobj)
		return;

	pobj->n_ref++;
}

void te_decref(te_obj_st* pobj)
{
	if (!pobj)
		return;

	pobj->n_ref--;
	if (pobj->n_ref)
		return;

	pobj->is_valid = false;
	pobj->ty->ty_del(pobj);
	free(pobj);
}

void te_incref_s(te_obj_st* pobj)
{
	if (!pobj)
		return;
	te_incref(pobj);
}

void te_decref_s(te_obj_st* pobj)
{
	if (!pobj)
		return;
	te_decref(pobj);
}
