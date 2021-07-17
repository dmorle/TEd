#ifndef TEDLANG_OS_CPTR_H
#define TEDLANG_OS_CPTR_H

#include <tedlang/tedl.h>

te_obj_st* te_cptr_new();
void te_cptr_del(te_obj_st* pself);
bool te_cptr_bool(te_obj_st* pself);
const char* te_cptr_repr(te_obj_st* pself);
te_obj_st* te_cptr_call(te_obj_st* pself, const te_fnargs_st* pargs);

te_obj_st* te_cptr_iadd(te_obj_st** ppself, te_obj_st* prval);
te_obj_st* te_cptr_isub(te_obj_st** ppself, te_obj_st* prval);
te_obj_st* te_cptr_add(te_obj_st* pself, te_obj_st* prval);
te_obj_st* te_cptr_sub(te_obj_st* pself, te_obj_st* prval);
te_obj_st* te_cptr_eq(te_obj_st* pself, te_obj_st* prval);
te_obj_st* te_cptr_ne(te_obj_st* pself, te_obj_st* prval);
te_obj_st* te_cptr_lt(te_obj_st* pself, te_obj_st* prval);
te_obj_st* te_cptr_gt(te_obj_st* pself, te_obj_st* prval);
te_obj_st* te_cptr_le(te_obj_st* pself, te_obj_st* prval);
te_obj_st* te_cptr_ge(te_obj_st* pself, te_obj_st* prval);

const extern te_type_st _te_cptr_ty;

typedef struct
{
	te_obj_st super;
	void* ptr;
}
te_cptr_st;

#endif
