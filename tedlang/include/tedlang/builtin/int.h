#ifndef TE_INT_H
#define TE_INT_H

#include <stdbool.h>

#include <tedlang/tedl.h>
#include <tedlang/core/obj.h>

TEDLANG_API te_obj_st* te_int_new();
TEDLANG_API void te_int_del(te_obj_st* pself);

TEDLANG_API bool te_int_bool(te_obj_st* pself);
TEDLANG_API int64_t te_int_int(te_obj_st* pself);
TEDLANG_API const char* te_int_repr(te_obj_st* pself);
TEDLANG_API te_iterable_st* te_int_iter(te_obj_st* pself);

TEDLANG_API te_obj_st* te_int_iadd(te_obj_st** ppself, te_obj_st* plval);
TEDLANG_API te_obj_st* te_int_isub(te_obj_st** ppself, te_obj_st* plval);
TEDLANG_API te_obj_st* te_int_imul(te_obj_st** ppself, te_obj_st* plval);
TEDLANG_API te_obj_st* te_int_idiv(te_obj_st** ppself, te_obj_st* plval);
TEDLANG_API te_obj_st* te_int_imod(te_obj_st** ppself, te_obj_st* plval);
TEDLANG_API te_obj_st* te_int_add(te_obj_st* pself, te_obj_st* plval);
TEDLANG_API te_obj_st* te_int_sub(te_obj_st* pself, te_obj_st* plval);
TEDLANG_API te_obj_st* te_int_mul(te_obj_st* pself, te_obj_st* plval);
TEDLANG_API te_obj_st* te_int_div(te_obj_st* pself, te_obj_st* plval);
TEDLANG_API te_obj_st* te_int_mod(te_obj_st* pself, te_obj_st* plval);
TEDLANG_API te_obj_st* te_int_eq(te_obj_st* pself, te_obj_st* plval);
TEDLANG_API te_obj_st* te_int_ne(te_obj_st* pself, te_obj_st* plval);
TEDLANG_API te_obj_st* te_int_lt(te_obj_st* pself, te_obj_st* plval);
TEDLANG_API te_obj_st* te_int_gt(te_obj_st* pself, te_obj_st* plval);
TEDLANG_API te_obj_st* te_int_le(te_obj_st* pself, te_obj_st* plval);
TEDLANG_API te_obj_st* te_int_ge(te_obj_st* pself, te_obj_st* plval);

extern te_type_st _te_int_ty;

typedef struct
{
	te_obj_st super;
	int64_t val;
}
te_int_st;

#endif
