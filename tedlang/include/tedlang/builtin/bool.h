#ifndef TE_BOOL_H
#define TE_BOOL_H

#include <tedlang/tedl.h>
#include <tedlang/core/obj.h>

TE_API te_obj_st* te_bool_new();
TE_API void te_bool_del(te_obj_st* pself);

TE_API const char* te_bool_str(te_obj_st* pself);
TE_API bool te_bool_bool(te_obj_st* pself);

static te_type_st _te_bool_ty = {
	.ty_new = te_bool_new,
	.ty_del = te_bool_del,
	.ty_str = te_bool_str,
	.ty_bool = te_bool_bool
};

typedef struct
{
	te_obj_st;
	bool val;
}
te_bool_st;

#endif
