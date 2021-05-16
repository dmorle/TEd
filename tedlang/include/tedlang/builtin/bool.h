#ifndef TEDLANG_BOOL_H
#define TEDLANG_BOOL_H

#include <tedlang/tedl.h>
#include <tedlang/core/obj.h>

TEDLANG_API te_obj_st* te_bool_new();
TEDLANG_API void te_bool_del(te_obj_st* pself);

TEDLANG_API const char* te_bool_repr(te_obj_st* pself);
TEDLANG_API bool te_bool_bool(te_obj_st* pself);

extern te_type_st _te_bool_ty;

typedef struct
{
	te_obj_st super;
	bool val;
}
te_bool_st;

#endif
