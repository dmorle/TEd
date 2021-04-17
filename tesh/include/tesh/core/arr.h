#ifndef TI_ARR_H
#define TI_ARR_H

#include <tish/core/obj.h>

typedef struct
{
	te_obj_st super;
}
te_arr_st;

void _te_arr_dealloc(te_arr_st* parr);

#endif
