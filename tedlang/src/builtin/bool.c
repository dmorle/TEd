#define TEDLANG_SRC

#include <stdlib.h>

#include <tedlang/core/obj.h>
#include <tedlang/core/eval.h>
#include <tedlang/builtin/bool.h>
#include <tedlang/util/string.h>

te_type_st _te_bool_ty = {
	.name = "bool",
	.objsize = sizeof(te_bool_st),
	.ty_new = te_bool_new,
	.ty_del = te_bool_del,
	.ty_repr = te_bool_repr,
	.ty_bool = te_bool_bool
};

#define self (*(te_bool_st*)pself)
#define CHECK_TYPE_RET(ret) if (pself->ty != &_te_bool_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_TYPE CHECK_TYPE_RET(NULL)
#define CHECK_PTYPE_RET(ret) if ((*ppself)->ty != &_te_bool_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_PTYPE CHECK_PTYPE_RET(NULL)

te_obj_st* te_bool_new()
{
	te_bool_st* pbool = (te_bool_st*)malloc(sizeof(te_bool_st));
	if (!pbool)
		return te_seterr("Out of memory");

	te_obj_new(pbool, &_te_bool_ty);
	pbool->val = false;
	return (te_obj_st*)pbool;
}

void te_bool_del(te_obj_st* pself)
{
	CHECK_TYPE_RET();
	te_obj_del(pself);
}

const char* te_bool_repr(te_obj_st* pself)
{
	CHECK_TYPE;
	static char reprbuf[6];
	if (self.val) memcpy(reprbuf, "true" , sizeof("true" ));
	else          memcpy(reprbuf, "false", sizeof("false"));
	return reprbuf;
}

bool te_bool_bool(te_obj_st* pself)
{
	CHECK_TYPE_RET(false);
	return self.val;
}
