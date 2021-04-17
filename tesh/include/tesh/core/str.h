#ifndef TI_STR_H
#define TI_STR_H

#include <tish/core/obj.h>

typedef struct
{
	te_obj_st super;
}
str_st;

void _te_str_dealloc(str_st* pstr);

#endif
