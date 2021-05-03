#ifndef TE_OBJ_H
#define TE_OBJ_H

#include <stdint.h>

#include <telang/telapi.h>

typedef enum
{
	TE_INT = 0,
	TE_ARR,
	TE_STR
}
te_type_et;

typedef struct
{
	te_type_et ty;
	size_t n_ref;
}
te_obj_st;

TE_API void te_incref(te_obj_st* pObj);
TE_API void te_decref(te_obj_st* pObj);

TE_API void te_passarg(te_obj_st* pObj);

#endif
