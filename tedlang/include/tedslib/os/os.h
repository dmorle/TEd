#ifndef TEDLANG_OS_H
#define TEDLANG_OS_H

#include <stdbool.h>

#include <tedlang/tedl.h>

bool te_open_lib_bool(te_obj_st* pself);
te_obj_st* te_open_lib_call(te_obj_st* pself, const te_fnargs_st* pargs);

const extern te_type_st _te_open_lib_ty;

bool te_close_lib_bool(te_obj_st* pself);
te_obj_st* te_close_lib_call(te_obj_st* pself, const te_fnargs_st* pargs);

const extern te_type_st _te_close_lib_ty;

bool te_load_proc_bool(te_obj_st* pself);
te_obj_st* te_load_proc_call(te_obj_st* pself, const te_fnargs_st* pargs);

const extern te_type_st _te_load_proc_ty;

TEDLANG_MODINIT te_scope_st* te_mod_init(te_scope_st* pscope);

#endif
