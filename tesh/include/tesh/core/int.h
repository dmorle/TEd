#ifndef TI_INT_H
#define TI_INT_H

#include <tish/core/obj.h>

typedef struct
{
	te_obj_st super;
}
te_int_st;

void _te_int_dealloc(te_int_st* pint);

#endif
