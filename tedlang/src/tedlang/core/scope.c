#define TEDLANG_SRC

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include <tedlang/core/scope.h>

#define FNV_PRIME 0x00000100000001B3ULL
#define FNV_OFFSET_BASIS 0XCBF29CE484222325ULL

inline uint64_t fnv_1a(const char* s)
{
	uint64_t h = FNV_OFFSET_BASIS;
	for (char* c = s; *c; c++)
		h = (h * FNV_PRIME) ^ *c;
	return h;
}

te_scope_st* scope_rehash(te_scope_st* pself)
{
	const static size_t incrate = 4;

	// setup
	size_t nsz = pself->n_buckets * incrate;
	_te_scope_bkt_st* npbuckets = (_te_scope_bkt_st*)malloc(sizeof(_te_scope_bkt_st) * nsz);
	if (!npbuckets)
		return NULL;

	// rehashing
	_te_scope_bktnd_st* pnext = NULL;
	for (_te_scope_bktnd_st** ppnd = pself->pbuckets; ppnd < pself->pbuckets + pself->n_buckets; ppnd++)
		for (_te_scope_bktnd_st* pnd = *ppnd; pnd; pnd = pnext)
		{
			pnext = pnd->pnext;

			uint64_t h = fnv_1a(pnd->name) % nsz;
			if (!npbuckets[h])
				npbuckets[h] = pnd;
			else
			{
				_te_scope_bktnd_st* npnd = npbuckets[h];
				for (; npnd->pnext; npnd = npnd->pnext);
				npnd->pnext = pnd;
			}
			pnd->pnext = NULL;
		}

	// cleanup
	free(pself->pbuckets);
	pself->pbuckets = npbuckets;
	pself->n_buckets = nsz;
	pself->curr_lf /= incrate;
	
	return pself;
}

void scope_bktnd_del(_te_scope_bktnd_st* pnd)
{
	if (!pnd)
		return;
	free(pnd->name);
	te_decref(pnd->pobj);
	_te_scope_bktnd_st* pnext = pnd->pnext;
	free(pnd);
	scope_bktnd_del(pnext);
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

void te_scope_free(te_scope_st* pself)
{
	te_scope_del(pself);
	free(pself);
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
	for (_te_scope_bkt_st* pbkt = pself->pbuckets; pbkt < pself->pbuckets + sz; pbkt++)
		*pbkt = NULL;

	pself->pparent = pparent;
	pself->target_lf = lf;
	pself->curr_lf = 0;
	pself->n_buckets = sz;
}

void te_scope_del(te_scope_st* pself)
{
	for (_te_scope_bkt_st* pbkt = pself->pbuckets; pbkt < pself->pbuckets + pself->n_buckets; pbkt++)
		scope_bktnd_del(*pbkt);
	free(pself->pbuckets);
}

inline te_obj_st** scope_get(te_scope_st* pself, const char* name, uint64_t h)
{
	if (!pself)
		return NULL;

	for (_te_scope_bktnd_st* pnd = pself->pbuckets[h]; pnd; pnd = pnd->pnext)
		if (!strcmp(name, pnd->name))
			return &(pnd->pobj);

	return scope_get(pself->pparent, name, h);
}

te_obj_st** te_scope_get(te_scope_st* pself, const char* name)
{
	return scope_get(pself, name, fnv_1a(name) % pself->n_buckets);
}

inline te_scope_st* scope_set(te_scope_st* pself, const char* name, te_obj_st* pobj, uint64_t h)
{
	if (!pself)
		return NULL;

	for (_te_scope_bktnd_st* pnd = pself->pbuckets[h]; pnd; pnd = pnd->pnext)
		if (!strcmp(name, pnd->name))
		{
			te_decref(pnd->pobj);
			te_incref(pobj);
			pnd->pobj = pobj;
			return pself;
		}
	
	return scope_set(pself->pparent, name, pobj, h);
}

te_scope_st* te_scope_set(te_scope_st* pself, const char* name, te_obj_st* pobj)
{
	uint64_t h = fnv_1a(name) % pself->n_buckets;

	if (scope_set(pself, name, pobj, h))
		return pself;

	// adding a new element
	_te_scope_bktnd_st** ppnd = pself->pbuckets + h;
	while (*ppnd)
		ppnd = &(*ppnd)->pnext;
	
	*ppnd = (_te_scope_bktnd_st*)malloc(sizeof(_te_scope_bktnd_st));
	if (!*ppnd)
		return NULL;

	_te_scope_bktnd_st* pnd = *ppnd;
	size_t nmsz = strlen(name);
	pnd->name = malloc(sizeof(char) * (nmsz + 1));
	if (!pnd->name)
	{
		free(pnd);
		*ppnd = NULL;
		return NULL;
	}
	memcpy(pnd->name, name, nmsz + 1);
	te_incref(pobj);
	pnd->pobj = pobj;
	pnd->pnext = NULL;

	pself->curr_lf = (pself->curr_lf * pself->n_buckets + 1) / pself->n_buckets;
	if (pself->curr_lf > pself->target_lf)
		return scope_rehash(pself);

	return pself;
}

inline void scope_rm(te_scope_st* pself, const char* name, uint64_t h)
{
	if (!pself)
		return;

	// seperate proc for if its the first element in the list
	_te_scope_bktnd_st* pnd = pself->pbuckets[h];
	if (!pnd)
		return;

	if (!strcmp(name, pnd->name))
	{
		// TODO: switch around the pointers
	}

	// general proc for an element along the chain
	for (; pnd->pnext; pnd = pnd->pnext)
		if (!strcmp(name, pnd->pnext->name))
		{
			_te_scope_bktnd_st* ptmpnd = pnd->pnext;
			pnd->pnext = ptmpnd->pnext;
			ptmpnd->pnext = NULL;
			scope_bktnd_del(ptmpnd);
			pself->curr_lf = (pself->curr_lf * pself->n_buckets - 1) / pself->n_buckets;
			return;
		}

	scope_rm(pself->pparent, name, h);
}

void te_scope_rm(te_scope_st* pself, const char* name)
{
	scope_rm(pself, name, fnv_1a(name));
}
