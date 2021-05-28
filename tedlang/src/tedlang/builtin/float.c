#define TEDLANG_SRC

#include <math.h>
#include <stdlib.h>

#include <tedlang/core/obj.h>
#include <tedlang/core/eval.h>
#include <tedlang/builtin/bool.h>
#include <tedlang/builtin/char.h>
#include <tedlang/builtin/int.h>
#include <tedlang/builtin/float.h>
#include <tedlang/util/string.h>

te_type_st _te_float_ty = {
	.name = "float",
	.objsize = sizeof(te_float_st),
	.ty_new = te_float_new,
	.ty_del = te_float_del,
	.ty_repr = te_float_repr,
	.ty_iadd = te_float_iadd,
	.ty_isub = te_float_isub,
	.ty_imul = te_float_imul,
	.ty_idiv = te_float_idiv,
	.ty_iexp = te_float_iexp,
	.ty_add = te_float_add,
	.ty_sub = te_float_sub,
	.ty_mul = te_float_mul,
	.ty_div = te_float_div,
	.ty_exp = te_float_exp,
	.ty_eq = te_float_eq,
	.ty_ne = te_float_ne,
	.ty_lt = te_float_lt,
	.ty_gt = te_float_gt,
	.ty_le = te_float_le,
	.ty_ge = te_float_ge
};

#define self (*(te_float_st*)pself)
#define this ((te_float_st*)*ppself)
#define crval (*(te_char_st*)prval)
#define irval (*(te_int_st*)prval)
#define frval (*(te_float_st*)prval)
#define CHECK_TYPE_RET(ret) if (pself->ty != &_te_float_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_TYPE CHECK_TYPE_RET(NULL)
#define CHECK_PTYPE_RET(ret) if ((*ppself)->ty != &_te_float_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_PTYPE CHECK_PTYPE_RET(NULL)

te_obj_st* te_float_new()
{
	te_float_st* pfloat = (te_float_st*)malloc(sizeof(te_float_st));
	if (!pfloat)
		return te_seterr("Out of memory");

	te_obj_new(pfloat, &_te_float_ty);
	pfloat->val = 0;
	return pfloat;
}

void te_float_del(te_obj_st* pself)
{
	CHECK_TYPE_RET();
	te_obj_del(pself);
}

const char* te_float_repr(te_obj_st* pself)
{
	CHECK_TYPE;

	// TODO: Implement float to string
}

te_obj_st* te_float_iadd(te_obj_st** ppself, te_obj_st* prval)
{
	CHECK_PTYPE;
	if (this->super.ty == &_te_char_ty)
	{
		this->val += crval.ch;
		return this;
	}
	if (this->super.ty == &_te_int_ty)
	{
		this->val += irval.val;
		return this;
	}
	if (this->super.ty == &_te_float_ty)
	{
		this->val += frval.val;
		return this;
	}
	return te_seterr("Invalid type '%s' for numeric addition", prval->ty->name);
}

te_obj_st* te_float_isub(te_obj_st** ppself, te_obj_st* prval)
{
	CHECK_PTYPE;
	if (this->super.ty == &_te_char_ty)
	{
		this->val -= crval.ch;
		return this;
	}
	if (this->super.ty == &_te_int_ty)
	{
		this->val -= irval.val;
		return this;
	}
	if (this->super.ty == &_te_float_ty)
	{
		this->val -= frval.val;
		return this;
	}
	return te_seterr("Invalid type '%s' for numeric subtraction", prval->ty->name);
}

te_obj_st* te_float_imul(te_obj_st** ppself, te_obj_st* prval)
{
	CHECK_PTYPE;
	if (this->super.ty == &_te_char_ty)
	{
		this->val *= crval.ch;
		return this;
	}
	if (this->super.ty == &_te_int_ty)
	{
		this->val *= irval.val;
		return this;
	}
	if (this->super.ty == &_te_float_ty)
	{
		this->val *= frval.val;
		return this;
	}
	return te_seterr("Invalid type '%s' for numeric multiplication", prval->ty->name);
}

te_obj_st* te_float_idiv(te_obj_st** ppself, te_obj_st* prval)
{
	CHECK_PTYPE;
	if (this->super.ty == &_te_char_ty)
	{
		this->val /= crval.ch;
		return this;
	}
	if (this->super.ty == &_te_int_ty)
	{
		this->val /= irval.val;
		return this;
	}
	if (this->super.ty == &_te_float_ty)
	{
		this->val /= frval.val;
		return this;
	}
	return te_seterr("Invalid type '%s' for numeric division", prval->ty->name);
}

te_obj_st* te_float_iexp(te_obj_st** ppself, te_obj_st* prval)
{
	CHECK_PTYPE;
	if (this->super.ty == &_te_char_ty)
	{
		this->val = pow(this->val, crval.ch);
		return this;
	}
	if (this->super.ty == &_te_int_ty)
	{
		this->val = pow(this->val, irval.val);
		return this;
	}
	if (this->super.ty == &_te_float_ty)
	{
		this->val = pow(this->val, frval.val);
		return this;
	}
	return te_seterr("Invalid type '%s' for numeric exponentiation", prval->ty->name);
}

te_obj_st* te_float_add(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	if (pself->ty == &_te_char_ty)
	{
		te_float_st* pfloat = (te_float_st*)te_float_new();
		if (te_haserr())
			return NULL;
		pfloat->val = self.val + crval.ch;
		return pself;
	}
	if (pself->ty == &_te_int_ty)
	{
		te_float_st* pfloat = (te_float_st*)te_float_new();
		if (te_haserr())
			return NULL;
		pfloat->val = self.val + irval.val;
		return pself;
	}
	if (pself->ty == &_te_float_ty)
	{
		te_float_st* pfloat = (te_float_st*)te_float_new();
		if (te_haserr())
			return NULL;
		pfloat->val = self.val + frval.val;
		return pself;
	}
	return te_seterr("Invalid type '%s' for numeric addition", prval->ty->name);
}

te_obj_st* te_float_sub(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	if (pself->ty == &_te_char_ty)
	{
		te_float_st* pfloat = (te_float_st*)te_float_new();
		if (te_haserr())
			return NULL;
		pfloat->val = self.val - crval.ch;
		return pself;
	}
	if (pself->ty == &_te_int_ty)
	{
		te_float_st* pfloat = (te_float_st*)te_float_new();
		if (te_haserr())
			return NULL;
		pfloat->val = self.val - irval.val;
		return pself;
	}
	if (pself->ty == &_te_float_ty)
	{
		te_float_st* pfloat = (te_float_st*)te_float_new();
		if (te_haserr())
			return NULL;
		pfloat->val = self.val - frval.val;
		return pself;
	}
	return te_seterr("Invalid type '%s' for numeric subtraction", prval->ty->name);
}

te_obj_st* te_float_mul(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	if (pself->ty == &_te_char_ty)
	{
		te_float_st* pfloat = (te_float_st*)te_float_new();
		if (te_haserr())
			return NULL;
		pfloat->val = self.val * crval.ch;
		return pself;
	}
	if (pself->ty == &_te_int_ty)
	{
		te_float_st* pfloat = (te_float_st*)te_float_new();
		if (te_haserr())
			return NULL;
		pfloat->val = self.val * irval.val;
		return pself;
	}
	if (pself->ty == &_te_float_ty)
	{
		te_float_st* pfloat = (te_float_st*)te_float_new();
		if (te_haserr())
			return NULL;
		pfloat->val = self.val * frval.val;
		return pself;
	}
	return te_seterr("Invalid type '%s' for numeric multiplication", prval->ty->name);
}

te_obj_st* te_float_div(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	if (pself->ty == &_te_char_ty)
	{
		te_float_st* pfloat = (te_float_st*)te_float_new();
		if (te_haserr())
			return NULL;
		pfloat->val = self.val / crval.ch;
		return pself;
	}
	if (pself->ty == &_te_int_ty)
	{
		te_float_st* pfloat = (te_float_st*)te_float_new();
		if (te_haserr())
			return NULL;
		pfloat->val = self.val / irval.val;
		return pself;
	}
	if (pself->ty == &_te_float_ty)
	{
		te_float_st* pfloat = (te_float_st*)te_float_new();
		if (te_haserr())
			return NULL;
		pfloat->val = self.val / frval.val;
		return pself;
	}
	return te_seterr("Invalid type '%s' for numeric division", prval->ty->name);
}

te_obj_st* te_float_exp(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	if (pself->ty == &_te_char_ty)
	{
		te_float_st* pfloat = (te_float_st*)te_float_new();
		if (te_haserr())
			return NULL;
		pfloat->val = pow(self.val, crval.ch);
		return pself;
	}
	if (pself->ty == &_te_int_ty)
	{
		te_float_st* pfloat = (te_float_st*)te_float_new();
		if (te_haserr())
			return NULL;
		pfloat->val = pow(self.val, irval.val);
		return pself;
	}
	if (pself->ty == &_te_float_ty)
	{
		te_float_st* pfloat = (te_float_st*)te_float_new();
		if (te_haserr())
			return NULL;
		pfloat->val = pow(self.val, frval.val);
		return pself;
	}
	return te_seterr("Invalid type '%s' for numeric exponentiation", prval->ty->name);
}

#define EQ_ERR 1e-9

te_obj_st* te_float_eq(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	if (pself->ty == &_te_char_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = (self.val * (1 - EQ_ERR) < crval.ch && crval.ch < self.val * (1 + EQ_ERR));
		return pbool;
	}
	if (pself->ty == &_te_int_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = (self.val * (1 - EQ_ERR) < irval.val && irval.val < self.val * (1 + EQ_ERR));
		return pbool;
	}
	if (pself->ty == &_te_float_ty)
	{
		te_float_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = (self.val * (1 - EQ_ERR) < frval.val && frval.val < self.val * (1 + EQ_ERR));
		return pbool;
	}
	return te_seterr("Invalid type '%s' for numeric comparison", prval->ty->name);
}

te_obj_st* te_float_ne(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	if (pself->ty == &_te_char_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = (crval.ch < self.val * (1 - EQ_ERR) || self.val * (1 + EQ_ERR) < crval.ch);
		return pbool;
	}
	if (pself->ty == &_te_int_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = (irval.val < self.val * (1 - EQ_ERR) || self.val * (1 + EQ_ERR) < irval.val);
		return pbool;
	}
	if (pself->ty == &_te_float_ty)
	{
		te_float_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = (frval.val < self.val * (1 - EQ_ERR) || self.val * (1 + EQ_ERR) < frval.val);
		return pbool;
	}
	return te_seterr("Invalid type '%s' for numeric comparison", prval->ty->name);
}

te_obj_st* te_float_lt(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	if (pself->ty == &_te_char_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.val < crval.ch;
		return pbool;
	}
	if (pself->ty == &_te_int_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.val < irval.val;
		return pbool;
	}
	if (pself->ty == &_te_float_ty)
	{
		te_float_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.val < frval.val;
		return pbool;
	}
	return te_seterr("Invalid type '%s' for numeric comparison", prval->ty->name);
}

te_obj_st* te_float_gt(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	if (pself->ty == &_te_char_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.val > crval.ch;
		return pbool;
	}
	if (pself->ty == &_te_int_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.val > irval.val;
		return pbool;
	}
	if (pself->ty == &_te_float_ty)
	{
		te_float_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.val > frval.val;
		return pbool;
	}
	return te_seterr("Invalid type '%s' for numeric comparison", prval->ty->name);
}

te_obj_st* te_float_le(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	if (pself->ty == &_te_char_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.val <= crval.ch;
		return pbool;
	}
	if (pself->ty == &_te_int_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.val <= irval.val;
		return pbool;
	}
	if (pself->ty == &_te_float_ty)
	{
		te_float_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.val <= frval.val;
		return pbool;
	}
	return te_seterr("Invalid type '%s' for numeric comparison", prval->ty->name);
}

te_obj_st* te_float_ge(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	if (pself->ty == &_te_char_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.val >= crval.ch;
		return pbool;
	}
	if (pself->ty == &_te_int_ty)
	{
		te_bool_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.val >= irval.val;
		return pbool;
	}
	if (pself->ty == &_te_float_ty)
	{
		te_float_st* pbool = (te_bool_st*)te_bool_new();
		if (te_haserr())
			return NULL;
		pbool->val = self.val >= frval.val;
		return pbool;
	}
	return te_seterr("Invalid type '%s' for numeric comparison", prval->ty->name);
}
