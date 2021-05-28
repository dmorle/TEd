#ifndef TEDLANG_IO_H
#define TEDLANG_IO_H

#include <stdbool.h>

#include <tedlang/tedl.h>

te_obj_st* te_print_new();
bool te_print_bool(te_obj_st* pself);
te_obj_st* te_print_call(te_obj_st* pself, const te_fnargs_st* pargs);

extern te_type_st _te_print_ty;

typedef struct
{
	te_obj_st super;
}
te_print_st;

TEDLANG_MODINIT te_scope_st* te_mod_init(te_scope_st* pscope);

#endif
