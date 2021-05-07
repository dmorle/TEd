#define TE_SRC

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include <tedlang/core/scope.h>

#define FNV_PRIME 0x00000100000001B3ULL
#define FNV_OFFSET_BASIS 0XCBF29CE484222325ULL

inline uint64_t fnv_1a(char* s)
{
	uint64_t h = FNV_OFFSET_BASIS;
	for (char* c = s; c; c++)
		h = (h * FNV_PRIME) ^ *c;
	return h;
}

void scope_bktnd_del(_te_scope_bktnd_st* pnd)
{
	if (!pnd)
		return;
	free(pnd->name);
	te_decref(pnd->pobj);
	scope_bktnd_del(pnd->pnext);
}

te_scope_st* te_scope_alloc(te_scope_st* pparent, size_t sz, float lf)
{
	te_scope_st* pscope = (te_scope_st*)malloc(sizeof(te_scope_st));
	if (!pscope)
		return NULL;

	te_scope_st* npscope = te_scope_new(pscope, pparent, sz, lf);
	if (!npscope)
		free(pscope);
	return npscope;
}

te_scope_st* te_scope_new(te_scope_st* pself, te_scope_st* pparent, size_t sz, float lf)
{
	assert(sz);

	pself->pbuckets = (_te_scope_bkt_st*)malloc(sizeof(_te_scope_bkt_st) * sz);
	if (!pself->pbuckets)
	{
#ifdef _DEBUG
		pself->n_buckets = 0;
#endif // _DEBUG
		return NULL;
	}
	for (_te_scope_bkt_st* pbkt = pself->pbuckets; pbkt < pself->pbuckets + pself->n_buckets; pbkt++)
		*pbkt = NULL;

	pself->pparent = pparent;
	pself->target_lf = lf;
	pself->curr_lf = 0;
	pself->n_buckets = sz;
	te_scope_bkt_new(pself->pbuckets);
}

void te_scope_del(te_scope_st* pself)
{
	for (_te_scope_bkt_st* pbkt = pself->pbuckets; pbkt < pself->pbuckets + pself->n_buckets; pbkt++)
		scope_bktnd_del(*pbkt);
	free(pself->pbuckets);
}

inline te_obj_st* scope_get(te_scope_st* pself, char* name, uint64_t h)
{
	if (!pself)
		return NULL;

	for (_te_scope_bktnd_st* pnd = pself->pbuckets[h]; pnd; pnd = pnd->pnext)
		if (!strcmp(name, pnd->name))
			return pnd->pobj;

	return scope_get(pself->pparent, name, h);
}

te_obj_st* te_scope_get(te_scope_st* pself, char* name)
{
	return scope_get(pself, name, fnv_1a(name) % pself->n_buckets);
}

inline te_scope_st* scope_set(te_scope_st* pself, char* name, te_obj_st* pobj, uint64_t h)
{
	if (!pself)
		return NULL;

	for (_te_scope_bktnd_st* pnd = pself->pbuckets[h]; pnd; pnd = pnd->pnext)
		if (!strcmp(name, pnd->name))
		{
			te_decref(pnd->pobj);
			pnd->pobj = pobj;
			return pself;
		}
	
	return scope_set(pself->pparent, name, pobj, h);
}

te_scope_st* te_scope_set(te_scope_st* pself, char* name, te_obj_st* pobj)
{
	return scope_set(pself, name, pobj, fnv_1a(name) % pself->n_buckets);
}
