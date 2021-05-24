#ifndef TEDLANG_FLOAT_H
#define TEDLANG_FLOAT_H

#include <tedlang/tedl.h>
#include <tedlang/core/obj.h>

TEDLANG_API te_obj_st* te_float_new();
TEDLANG_API void te_float_del(te_obj_st* pself);

TEDLANG_API const char* te_float_repr(te_obj_st* pself);

TEDLANG_API te_obj_st* te_float_iadd(te_obj_st** ppself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_float_isub(te_obj_st** ppself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_float_imul(te_obj_st** ppself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_float_idiv(te_obj_st** ppself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_float_iexp(te_obj_st** ppself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_float_add(te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_float_sub(te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_float_mul(te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_float_div(te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_float_exp(te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_float_eq(te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_float_ne(te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_float_lt(te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_float_gt(te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_float_le(te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_float_ge(te_obj_st* pself, te_obj_st* prval);

extern te_type_st _te_float_ty;

typedef struct
{
	te_obj_st super;
	double val;
}
te_float_st;

#endif
