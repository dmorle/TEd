#define TEDLANG_SRC

#include <stdlib.h>

#include <tedlang/core/obj.h>
#include <tedlang/core/eval.h>
#include <tedlang/builtin/null.h>
#include <tedlang/builtin/bool.h>
#include <tedlang/util/string.h>

te_type_st _te_null_ty = {
	.name = "null",
	.objsize = sizeof(te_null_st),
	.ty_new = &te_null_new,
	.ty_del = &te_null_del,
	.ty_repr = &te_null_repr
};

#define CHECK_TYPE_RET(ret) if (pself->ty != &_te_null_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_TYPE CHECK_TYPE_RET(NULL)

te_obj_st* te_null_new()
{
	te_null_st* pnull = (te_null_st*)malloc(sizeof(te_null_st));
	if (!pnull)
		return te_seterr("Out of memory");

	te_obj_new((te_obj_st*)pnull, (te_obj_st*)&_te_null_ty);
	return (te_obj_st*)pnull;
}

void te_null_del(te_obj_st* pself)
{
	CHECK_TYPE_RET();
	te_obj_del(pself);
}

const char* te_null_repr(te_obj_st* pself)
{
	CHECK_TYPE;
	static const char* repr = "null";
	return repr;
}

te_obj_st* te_null_eq(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	
	te_bool_st* pbool = (te_bool_st*)te_bool_new();
	if (te_haserr())
		return NULL;

	if (prval->ty != &_te_null_ty)
		return (te_obj_st*)pbool;
	pbool->val = true;
	return (te_obj_st*)pbool;
}

te_obj_st* te_null_ne(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;

	te_bool_st* pbool = (te_bool_st*)te_bool_new();
	if (te_haserr())
		return NULL;

	if (prval->ty == &_te_null_ty)
		return (te_obj_st*)pbool;
	pbool->val = true;
	return (te_obj_st*)pbool;
}
