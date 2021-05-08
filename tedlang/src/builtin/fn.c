#define TEDLANG_SRC

#include <stdlib.h>
#include <string.h>

#include <tedlang/core/obj.h>
#include <tedlang/core/eval.h>
#include <tedlang/core/scope.h>
#include <tedlang/builtin/fn.h>

te_type_st _te_fn_ty = {
	.ty_new = te_fn_new,
	.ty_del = te_fn_del,
	.ty_repr = te_fn_repr,
	.ty_bool = te_fn_bool,
	.ty_call = te_fn_call
};

#define self (*(te_fn_st*)pself)
#define ASSERT_TYPE_RET(ret) if (pself->ty != &_te_fn_ty) { te_seterr("Invalid Type"); return ret; }
#define ASSERT_TYPE ASSERT_TYPE_RET(NULL)

te_obj_st* te_fn_new()
{
	te_fn_st* pfn = (te_fn_st*)malloc(sizeof(te_fn_st));
	if (!pfn)
		return NULL;

	te_obj_new(pfn, &_te_fn_ty);
	pfn->pbody = NULL;
	return (te_obj_st*)pfn;
}

void te_fn_del(te_obj_st* pself)
{
	ASSERT_TYPE_RET();

	te_obj_del(pself);
	if (self.pbody)
		_te_ast_del(self.pbody);
}

const char* te_fn_repr(te_obj_st* pself)
{
	static char prepr[64];

	ASSERT_TYPE;
	
	// TODO: Implementation
}

te_obj_st* te_fn_call(te_obj_st* pself, te_fnargs_st args)
{
	ASSERT_TYPE;

	te_scope_st lscope;
	if (!te_scope_new(&lscope, &global_scope, DEFAULT_SCOPESZ, DEFAULT_SCOPELF))
		return NULL;

	for (size_t i = 0; i < args.argc; i++)
	{
		args.pnames[i];
	}

	te_scope_del(&lscope);
}
