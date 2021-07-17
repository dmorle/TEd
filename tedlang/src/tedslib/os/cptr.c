#include <os/cptr.h>

const te_type_st _te_cptr_ty = {
	.name = "cptr",
	.objsize = sizeof(te_cptr_st),
	.ty_new = te_cptr_new,
	.ty_del = te_cptr_del,
	.ty_bool = te_cptr_bool,
	.ty_repr = te_cptr_repr,
	.ty_call = te_cptr_call,
	.ty_iadd = te_cptr_iadd,
	.ty_isub = te_cptr_isub,
	.ty_add = te_cptr_add,
	.ty_sub = te_cptr_sub,
	.ty_eq = te_cptr_eq,
	.ty_ne = te_cptr_ne,
	.ty_lt = te_cptr_lt,
	.ty_gt = te_cptr_gt,
	.ty_le = te_cptr_le,
	.ty_ge = te_cptr_ge
};

#define self (*(te_cptr_st*)pself)
#define rint (*(te_int_st*)prval)
#define rptr (*(te_cptr_st*)prval)
#define CHECK_TYPE_RET(ret) if (pself->ty != &_te_cptr_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_TYPE CHECK_TYPE_RET(NULL)
#define CHECK_PTYPE_RET(ret) if ((*ppself)->ty != &_te_cptr_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_PTYPE CHECK_PTYPE_RET(NULL)

te_obj_st* te_cptr_new()
{
	te_cptr_st* pptr = (te_cptr_st*)malloc(sizeof(te_cptr_st));
	if (!pptr)
		return te_seterr("Out of memory");

	te_obj_new(pptr, &_te_cptr_ty);
	pptr->ptr = NULL;
	return (te_obj_st*)pptr;
}

void te_cptr_del(te_obj_st* pself)
{
	CHECK_TYPE_RET();
	te_obj_del(pself);
}

bool te_cptr_bool(te_obj_st* pself)
{
	CHECK_TYPE_RET(false);
	return self.ptr;
}

const char* te_cptr_repr(te_obj_st* pself)
{
	static char hex[] = "0x0000000000000000";
	sprintf(hex, "%p", self.ptr);
	return hex;
}

te_obj_st* te_cptr_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	CHECK_TYPE;
	return te_seterr("Not Implemented");
}

te_obj_st* te_cptr_iadd(te_obj_st** ppself, te_obj_st* prval)
{
	CHECK_PTYPE;
	return te_seterr("Not Implemented");
}

te_obj_st* te_cptr_isub(te_obj_st** ppself, te_obj_st* prval)
{
	CHECK_PTYPE;
	return te_seterr("Not Implemented");
}

te_obj_st* te_cptr_add(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	return te_seterr("Not Implemented");
}

te_obj_st* te_cptr_sub(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	return te_seterr("Not Implemented");
}

te_obj_st* te_cptr_eq(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	return te_seterr("Not Implemented");
}

te_obj_st* te_cptr_ne(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	return te_seterr("Not Implemented");
}

te_obj_st* te_cptr_lt(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	return te_seterr("Not Implemented");
}

te_obj_st* te_cptr_gt(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	return te_seterr("Not Implemented");
}

te_obj_st* te_cptr_le(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	return te_seterr("Not Implemented");
}

te_obj_st* te_cptr_ge(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	return te_seterr("Not Implemented");
}
