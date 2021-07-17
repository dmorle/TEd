#ifndef TEDLANG_CHAR_H
#define TEDLANG_CHAR_H

#include <stdbool.h>

#include <tedlang/tedl.h>
#include <tedlang/core/obj.h>

TEDLANG_API te_obj_st* te_char_new();
TEDLANG_API void te_char_del(te_obj_st* pself);

TEDLANG_API bool te_char_bool(te_obj_st* pself);
TEDLANG_API int64_t te_char_int(te_obj_st* pself);
TEDLANG_API const char* te_char_repr(te_obj_st* pself);

TEDLANG_API te_obj_st* te_char_iadd(te_obj_st** ppself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_char_isub(te_obj_st** ppself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_char_add(te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_char_sub(te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_char_eq(te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_char_ne(te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_char_lt(te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_char_gt(te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_char_le(te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st* te_char_ge(te_obj_st* pself, te_obj_st* prval);

TEDLANG_API const extern te_type_st _te_char_ty;

typedef struct
{
	te_obj_st super;
	char ch;
}
te_char_st;

#endif
