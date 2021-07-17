#define TEDLANG_SRC

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <tedlang/core/obj.h>
#include <tedlang/core/eval.h>
#include <tedlang/core/scope.h>
#include <tedlang/builtin/fn.h>

const te_type_st _te_fn_ty = {
	.name = "fn",
	.objsize = sizeof(te_fn_st),
	.ty_new = &te_fn_new,
	.ty_del = &te_fn_del,
	.ty_repr = &te_fn_repr,
	.ty_bool = &te_fn_bool,
	.ty_call = &te_fn_call
};

#define self (*(te_fn_st*)pself)
#define CHECK_TYPE_RET(ret) if (pself->ty != &_te_fn_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_TYPE CHECK_TYPE_RET(NULL)

te_obj_st* te_fn_new()
{
	te_fn_st* pfn = (te_fn_st*)malloc(sizeof(te_fn_st));
	if (!pfn)
		return te_seterr("Out of memory");

	te_obj_new(pfn, &_te_fn_ty);
	pfn->pbody = NULL;
	pfn->argc = 0;
	pfn->ppargv = NULL;
	return (te_obj_st*)pfn;
}

void te_fn_del(te_obj_st* pself)
{
	CHECK_TYPE_RET();

	te_obj_del(pself);
	if (self.pbody)
	{
		_te_ast_del(self.pbody);
		free(self.pbody);
	}
	if (self.ppargv)
	{
		for (char** pe = self.ppargv; pe < self.ppargv + self.argc; pe++)
			free(*pe);
		free(self.ppargv);
	}
}

const char* te_fn_repr(te_obj_st* pself)
{
	CHECK_TYPE;
	static char prepr[64];
	
	// TODO: Implement repr for fns
}

bool te_fn_bool(te_obj_st* pself)
{
	CHECK_TYPE_RET(false);
	if (self.pbody)
		return true;
	return false;
}

te_obj_st* te_fn_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	CHECK_TYPE;

	if (self.argc != pargs->argc)
		return te_seterr("Function argument mismatch: expected %zu, recieved %zu", self.argc, pargs->argc);

	te_scope_st lscope;
	if (!te_scope_new(&lscope, &global_scope, DEFAULT_SCOPESZ, DEFAULT_SCOPELF))
		return NULL;
	for (size_t i = 0; i < pargs->argc; i++)
		if (!te_scope_set(&lscope, self.ppargv[i], pargs->ppargs[i]))
		{
			te_scope_del(&lscope);
			return te_seterr("Out of memory");
		}

	te_obj_st* pret = te_eval(&lscope, self.pbody);

	te_scope_del(&lscope);
	return pret;
}
