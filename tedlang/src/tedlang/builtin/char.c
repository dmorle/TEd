#define TEDLANG_SRC

#include <stdlib.h>
#include <stdbool.h>

#include <tedlang/core/obj.h>
#include <tedlang/core/eval.h>
#include <tedlang/builtin/bool.h>
#include <tedlang/builtin/char.h>
#include <tedlang/builtin/int.h>
#include <tedlang/builtin/float.h>
#include <tedlang/util/string.h>

te_type_st _te_char_ty = {
	.name = "char",
	.objsize = sizeof(te_char_st),
	.ty_new = &te_char_new,
	.ty_del = &te_char_del,
	.ty_bool = &te_char_bool,
	.ty_int = &te_char_int,
	.ty_repr = &te_char_repr,
	.ty_iadd = &te_char_iadd,
	.ty_isub = &te_char_isub,
	.ty_add = &te_char_add,
	.ty_sub = &te_char_sub,
	.ty_eq = &te_char_eq,
	.ty_ne = &te_char_ne,
	.ty_lt = &te_char_lt,
	.ty_gt = &te_char_gt,
	.ty_le = &te_char_le,
	.ty_ge = &te_char_ge
};

#define self (*(te_char_st*)pself)
#define this ((te_char_st*)*ppself)
#define crval (*(te_char_st*)prval)
#define irval (*(te_int_st*)prval)
#define frval (*(te_float_st*)prval)
#define CHECK_TYPE_RET(ret) if (pself->ty != &_te_char_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_TYPE CHECK_TYPE_RET(NULL)
#define CHECK_PTYPE_RET(ret) if ((*ppself)->ty != &_te_char_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_PTYPE CHECK_PTYPE_RET(NULL)

te_obj_st* te_char_new()
{
	te_char_st* pchar = (te_char_st*)malloc(sizeof(te_char_st));
	if (!pchar)
		return te_seterr("Out of memory");

	te_obj_new(pchar, &_te_char_ty);
	pchar->ch = '\0';
	return pchar;
}

void te_char_del(te_obj_st* pself)
{
	CHECK_TYPE_RET();
	te_obj_del(pself);
}

bool te_char_bool(te_obj_st* pself)
{
	CHECK_TYPE_RET(false);
	return (bool)(self.ch != 0);
}

int64_t te_char_int(te_obj_st* pself)
{
	CHECK_TYPE_RET(-1);
	return self.ch;
}

const char* te_char_repr(te_obj_st* pself)
{
	CHECK_TYPE;
	static char repr[2] = {0, 0};
	repr[0] = self.ch;
	return repr;
}

te_obj_st* te_char_iadd(te_obj_st** ppself, te_obj_st* prval)
{
	CHECK_PTYPE;
	if (this->super.ty == &_te_char_ty)
	{
		this->ch += crval.ch;
		return this;
	}
	if (this->super.ty == &_te_int_ty)
	{
		this->ch += irval.val;
		return this;
	}
	return te_seterr("Invalid type '%s' for character addition", prval->ty->name);
}

te_obj_st* te_char_isub(te_obj_st** ppself, te_obj_st* prval)
{
	CHECK_PTYPE;
	if (this->super.ty == &_te_char_ty)
	{
		this->ch += crval.ch;
		return this;
	}
	if (this->super.ty == &_te_int_ty)
	{
		this->ch += irval.val;
		return this;
	}
	return te_seterr("Invalid type '%s' for character addition", prval->ty->name);
}

te_obj_st* te_char_add(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	if (pself->ty == &_te_char_ty)
	{
		te_char_st* pchar = (te_char_st*)te_char_new();
		if (te_haserr())
			return NULL;
		pchar->ch = self.ch + crval.ch;
		return pchar;
	}
	if (pself->ty == &_te_int_ty)
	{
		te_int_st* pint = (te_int_st*)te_int_new();
		if (te_haserr())
			return NULL;
		pint->val = self.ch + irval.val;
		return pint;
	}
	if (pself->ty == &_te_float_ty)
	{
		te_float_st* pfloat = (te_float_st*)te_float_new();
		if (te_haserr())
			return NULL;
		pfloat->val = self.ch + frval.val;
		return pfloat;
	}
	return te_seterr("Invalid type '%s' for numeric addition", prval->ty->name);
}

te_obj_st* te_char_sub(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	if (pself->ty == &_te_char_ty)
	{
		te_char_st* pchar = (te_char_st*)te_char_new();
		if (te_haserr())
			return NULL;
		pchar->ch = self.ch - crval.ch;
		return pchar;
	}
	if (pself->ty == &_te_int_ty)
	{
		te_int_st* pint = (te_int_st*)te_int_new();
		if (te_haserr())
			return NULL;
		pint->val = self.ch + irval.val;
		return pint;
	}
	if (pself->ty == &_te_float_ty)
	{
		te_float_st* pfloat = (te_float_st*)te_float_new();
		if (te_haserr())
			return NULL;
		pfloat->val = self.ch - frval.val;
		return pfloat;
	}
	return te_seterr("Invalid type '%s' for numeric subtraction", prval->ty->name);
}

te_obj_st* te_char_eq(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	if (pself->ty == &_te_char_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.ch == crval.ch;
		return pbool;
	}
	if (pself->ty == &_te_int_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.ch == irval.val;
		return pbool;
	}
	return te_seterr("Invalid type '%s' for character comparsion", prval->ty->name);
}

te_obj_st* te_char_ne(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	if (pself->ty == &_te_char_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.ch != crval.ch;
		return pbool;
	}
	if (pself->ty == &_te_int_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.ch != irval.val;
		return pbool;
	}
	return te_seterr("Invalid type '%s' for character comparsion", prval->ty->name);
}

te_obj_st* te_char_lt(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	if (pself->ty == &_te_char_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.ch < crval.ch;
		return pbool;
	}
	if (pself->ty == &_te_int_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.ch < irval.val;
		return pbool;
	}
	return te_seterr("Invalid type '%s' for character comparsion", prval->ty->name);
}

te_obj_st* te_char_gt(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	if (pself->ty == &_te_char_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.ch > crval.ch;
		return pbool;
	}
	if (pself->ty == &_te_int_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.ch > irval.val;
		return pbool;
	}
	return te_seterr("Invalid type '%s' for character comparsion", prval->ty->name);
}

te_obj_st* te_char_le(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	if (pself->ty == &_te_char_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.ch <= crval.ch;
		return pbool;
	}
	if (pself->ty == &_te_int_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.ch <= irval.val;
		return pbool;
	}
	return te_seterr("Invalid type '%s' for character comparsion", prval->ty->name);
}

te_obj_st* te_char_ge(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	if (pself->ty == &_te_char_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.ch >= crval.ch;
		return pbool;
	}
	if (pself->ty == &_te_int_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.ch >= irval.val;
		return pbool;
	}
	return te_seterr("Invalid type '%s' for character comparsion", prval->ty->name);
}
