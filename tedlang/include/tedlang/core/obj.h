#ifndef TE_OBJ_H
#define TE_OBJ_H

#include <stdint.h>
#include <stdbool.h>

#include <tedlang/tedl.h>

typedef struct __te_obj te_obj_st;

typedef struct
{
	size_t argc;
	char** pnames;
	te_obj_st** ppargs;
}
te_fnargs_st;

TEDLANG_API void            te_del    (te_obj_st* pself);
TEDLANG_API te_obj_st*      te_cpy    (te_obj_st* pself);

TEDLANG_API bool            te_bool   (te_obj_st* pself);
TEDLANG_API int64_t         te_int    (te_obj_st* pself);
TEDLANG_API const char*     te_repr   (te_obj_st* pself);
TEDLANG_API te_obj_st*      te_call   (te_obj_st* pself, te_fnargs_st args);
TEDLANG_API te_obj_st*      te_start  (te_obj_st* pself);
TEDLANG_API te_obj_st*      te_next   (te_obj_st* pself, te_obj_st* piterctx);
TEDLANG_API te_obj_st*      te_not    (te_obj_st* pself);
TEDLANG_API te_obj_st**     te_idx    (te_obj_st* pself, te_obj_st* pidx);

TEDLANG_API te_obj_st*      te_assign (te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st*      te_iadd   (te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st*      te_isub   (te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st*      te_imul   (te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st*      te_idiv   (te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st*      te_imod   (te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st*      te_iexp   (te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st*      te_add    (te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st*      te_sub    (te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st*      te_mul    (te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st*      te_div    (te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st*      te_mod    (te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st*      te_exp    (te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st*      te_and    (te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st*      te_or     (te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st*      te_eq     (te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st*      te_ne     (te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st*      te_lt     (te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st*      te_gt     (te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st*      te_le     (te_obj_st* pself, te_obj_st* prval);
TEDLANG_API te_obj_st*      te_ge     (te_obj_st* pself, te_obj_st* prval);

typedef struct
{
	// basic data
	char name[64];
	size_t objsize;

	// Constructor / Destructor
	te_obj_st*      (*ty_new)    ();  // creates a new instance of the type with a reference count of 1
	void            (*ty_del)    (te_obj_st*);
	te_obj_st*      (*ty_cpy)    (te_obj_st*);

	// Special Operators
	bool            (*ty_bool)   (te_obj_st*);
	int64_t         (*ty_int)    (te_obj_st*);
	const char*     (*ty_repr)   (te_obj_st*);  // never free the data returned, it should be managed by the object's type or the object
	te_obj_st*      (*ty_call)   (te_obj_st*, te_fnargs_st);
	te_obj_st*      (*ty_start)  (te_obj_st*);  // returns an iterable context
	te_obj_st*      (*ty_next)   (te_obj_st*, te_obj_st*);  // retrieves the next element if it exists, NULL otherwise
	te_obj_st*      (*ty_not)    (te_obj_st*);
	te_obj_st**     (*ty_idx)    (te_obj_st*, te_obj_st*);  // produces an lval

	// Binary Operators
	te_obj_st*      (*ty_assign) (te_obj_st**, te_obj_st*);
	te_obj_st*      (*ty_iadd)   (te_obj_st**, te_obj_st*);
	te_obj_st*      (*ty_isub)   (te_obj_st**, te_obj_st*);
	te_obj_st*      (*ty_imul)   (te_obj_st**, te_obj_st*);
	te_obj_st*      (*ty_idiv)   (te_obj_st**, te_obj_st*);
	te_obj_st*      (*ty_imod)   (te_obj_st**, te_obj_st*);
	te_obj_st*      (*ty_iexp)   (te_obj_st**, te_obj_st*);

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

TEDLANG_API void te_obj_new(te_obj_st* pself, te_type_st* pty);
#define te_obj_del(pself)

TEDLANG_API void te_incref(te_obj_st* pobj);
TEDLANG_API void te_decref(te_obj_st* pobj);

#endif
