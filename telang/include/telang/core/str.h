#ifndef TE_STR_H
#define TE_STR_H

#include <telang/core/obj.h>

typedef struct
{
	te_obj_st super;
}
te_str_st;

void _te_str_dealloc(te_str_st* pstr);

#endif
