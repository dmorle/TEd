#ifndef TE_OBJ_H
#define TE_OBJ_H

#include <stdint.h>
#include <stdbool.h>

#include <tedlang/tedl.h>

typedef struct __te_obj te_obj_st;

typedef struct
{
	size_t argc;
	te_obj_st** ppargs;
}
te_fnargs_st;

typedef void* te_iter_ctx;
typedef struct
{
	te_iter_ctx (*start)(te_obj_st*);
	te_obj_st*  (*next)(te_iter_ctx);
}
te_iterable_st;

typedef struct
{
	// Constructor / Destructor
	te_obj_st*      (*ty_new)    ();
	void            (*ty_del)    (te_obj_st*);

	// Special Operators
	bool            (*ty_bool)   (te_obj_st*);
	int64_t         (*ty_int)    (te_obj_st*);
	const char*     (*ty_str)    (te_obj_st*);
	te_obj_st*      (*ty_call)   (te_fnargs_st);
	te_iterable_st* (*ty_iter)   (te_obj_st*);
	te_obj_st*      (*ty_not)    (te_obj_st*);
	te_obj_st*      (*ty_idx)    (te_obj_st*, te_obj_st*);

	// Binary Operators
	te_obj_st*      (*ty_assign) (te_obj_st*, te_obj_st*);
	te_obj_st*      (*ty_iadd)   (te_obj_st*, te_obj_st*);
	te_obj_st*      (*ty_isub)   (te_obj_st*, te_obj_st*);
	te_obj_st*      (*ty_imul)   (te_obj_st*, te_obj_st*);
	te_obj_st*      (*ty_idiv)   (te_obj_st*, te_obj_st*);
	te_obj_st*      (*ty_imod)   (te_obj_st*, te_obj_st*);
	te_obj_st*      (*ty_iexp)   (te_obj_st*, te_obj_st*);
	te_obj_st*      (*ty_add)    (te_obj_st*, te_obj_st*);
	te_obj_st*      (*ty_sub)    (te_obj_st*, te_obj_st*);
	te_obj_st*      (*ty_mul)    (te_obj_st*, te_obj_st*);
	te_obj_st*      (*ty_div)    (te_obj_st*, te_obj_st*);
	te_obj_st*      (*ty_mod)    (te_obj_st*, te_obj_st*);
	te_obj_st*      (*ty_exp)    (te_obj_st*, te_obj_st*);
	te_obj_st*      (*ty_and)    (te_obj_st*, te_obj_st*);
	te_obj_st*      (*ty_or)     (te_obj_st*, te_obj_st*);
	te_obj_st*      (*ty_eq)     (te_obj_st*, te_obj_st*);
	te_obj_st*      (*ty_ne)     (te_obj_st*, te_obj_st*);
	te_obj_st*      (*ty_lt)     (te_obj_st*, te_obj_st*);
	te_obj_st*      (*ty_gt)     (te_obj_st*, te_obj_st*);
	te_obj_st*      (*ty_le)     (te_obj_st*, te_obj_st*);
	te_obj_st*      (*ty_ge)     (te_obj_st*, te_obj_st*);
}
te_type_st;

typedef struct __te_obj
{
	te_type_st* ty;
	size_t n_ref;
	bool is_valid;
}
te_obj_st;

TEDLANG_API void te_incref(te_obj_st* pObj);
TEDLANG_API void te_decref(te_obj_st* pObj);

#endif
