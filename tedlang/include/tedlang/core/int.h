#ifndef TE_INT_H
#define TE_INT_H

#include <tedlang/core/obj.h>

typedef struct
{
	te_obj_st super;
}
te_int_st;

void _te_int_dealloc(te_int_st* pint);

#endif