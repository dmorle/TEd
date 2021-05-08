#ifndef TE_SCOPE_H
#define TE_SCOPE_H

#include <stdbool.h>

#include <tedlang/tedl.h>
#include <tedlang/core/obj.h>

typedef struct __te_scope_bktnd
{
	struct __te_scope_bktnd* pnext;
	char* name;
	te_obj_st* pobj;
}
_te_scope_bktnd_st, *_te_scope_bkt_st;

typedef struct __te_scope
{
	struct __te_scope* pparent;
	float target_lf;
	float curr_lf;
	size_t n_buckets;
	_te_scope_bkt_st* pbuckets;
}
te_scope_st;

te_scope_st te_global_scope;

TEDLANG_API te_scope_st* te_scope_alloc   (te_scope_st* pparent, size_t sz, float lf);
TEDLANG_API te_scope_st* te_scope_new     (te_scope_st* pself, te_scope_st* pparent, size_t sz, float lf);
TEDLANG_API void         te_scope_del     (te_scope_st* pself);

TEDLANG_API te_obj_st*   te_scope_get     (te_scope_st* pself, char* name);
// set will steals the pobj pointer, name is copied if nessisary
TEDLANG_API te_scope_st* te_scope_set     (te_scope_st* pself, char* name, te_obj_st* pobj);

#endif
