#ifndef TE_STR_H
#define TE_STR_H

#include <tedlang/tedl.h>
#include <tedlang/core/obj.h>

TEDLANG_API te_obj_st* te_str_new();
TEDLANG_API void te_str_del(te_obj_st* pself);

TEDLANG_API const char* te_str_repr(te_obj_st* pself);
TEDLANG_API te_obj_st* te_str_start(te_obj_st* pself);
TEDLANG_API te_obj_st* te_str_next(te_obj_st* pself, te_obj_st* pitctx);
TEDLANG_API bool te_str_bool(te_obj_st* pself);
TEDLANG_API te_obj_st* te_str_idx(te_obj_st* pself, te_obj_st* pidx);

TEDLANG_API te_obj_st* te_str_iadd(te_obj_st** ppself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_str_add(te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_str_eq(te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_str_ne(te_obj_st* pself, te_obj_st* prval);

extern te_type_st _te_str_ty;

typedef struct
{
	te_obj_st super;
	char* val;
}
te_str_st;

#endif
