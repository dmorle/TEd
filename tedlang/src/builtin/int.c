#define TEDLANG_SRC

#include <stdlib.h>

#include <tedlang/core/obj.h>
#include <tedlang/core/eval.h>
#include <tedlang/builtin/int.h>
#include <tedlang/util/string.h>

te_type_st _te_int_ty = {
	.name = "int",
	.objsize = sizeof(te_int_st),
	.ty_new = te_int_new,
	.ty_del = te_int_del,
	.ty_repr = te_int_repr,
	.ty_bool = te_int_bool,
	.ty_start = te_int_start,
	.ty_next = te_int_next,
	.ty_iadd = te_int_iadd,
	.ty_isub = te_int_isub,
	.ty_imul = te_int_imul,
	.ty_idiv = te_int_idiv,
	.ty_imod = te_int_imod,
	.ty_iexp = te_int_iexp,
	.ty_add = te_int_add,
	.ty_sub = te_int_sub,
	.ty_mul = te_int_mul,
	.ty_div = te_int_div,
	.ty_mod = te_int_mod,
	.ty_exp = te_int_exp,
	.ty_eq = te_int_eq,
	.ty_ne = te_int_ne,
	.ty_lt = te_int_lt,
	.ty_gt = te_int_gt,
	.ty_le = te_int_le,
	.ty_ge = te_int_ge
};

#define self (*(te_int_st*)pself)
#define CHECK_TYPE_RET(ret) if (pself->ty != &_te_int_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_TYPE CHECK_TYPE_RET(NULL)
#define CHECK_PTYPE_RET(ret) if ((*ppself)->ty != &_te_int_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_PTYPE CHECK_PTYPE_RET(NULL)

te_obj_st* te_int_new()
{
	te_int_st* pint = (te_int_st*)malloc(sizeof(te_int_st));
	if (!pint)
		return te_seterr("Out of memory");

	te_obj_new(pint, &_te_int_ty);
	pint->val = 0;
	return (te_obj_st*)pint;
}

void te_int_del(te_obj_st* pself)
{
	CHECK_TYPE_RET();
	te_obj_del(pself);
}

bool te_int_bool(te_obj_st* pself)
{
	CHECK_TYPE_RET(false);
	return (bool)(self.val != 0);
}

int64_t te_int_int(te_obj_st* pself)
{
	CHECK_TYPE_RET(-1);
	return self.val;
}

const char* te_int_repr(te_obj_st* pself)
{
	CHECK_TYPE;
	static char reprbuf[32]; // 32 digits is overkill for int64, no chance of overflow

	return te_itoa(reprbuf, self.val, 10);
}

te_obj_st* te_int_start(te_obj_st* pself)
{
	CHECK_TYPE;
	return te_int_new();
}

te_obj_st* te_int_next(te_obj_st* pself, te_obj_st* pitctx)
{
	CHECK_TYPE;

	if (((te_int_st*)pitctx)->val >= self.val)
		return NULL;
	
	((te_int_st*)pitctx)->val++;
	te_incref(pitctx);
	return pitctx;
}

te_obj_st* te_int_iadd(te_obj_st** ppself, te_obj_st* prval)
{
	CHECK_PTYPE;
	if (prval->ty == &_te_int_ty)
	{
		(*(te_int_st**)ppself)->val += ((te_int_st*)prval)->val;
		return *ppself;
	}
	return te_seterr("Invalid l-value type '%s' for integer addition", prval->ty->name);
}

te_obj_st* te_int_isub(te_obj_st** ppself, te_obj_st* prval)
{
	CHECK_PTYPE;
	if (prval->ty == &_te_int_ty)
	{
		(*(te_int_st**)ppself)->val -= ((te_int_st*)prval)->val;
		return *ppself;
	}
	return te_seterr("Invalid l-value type '%s' for integer subtraction", prval->ty->name);
}

te_obj_st* te_int_imul(te_obj_st** ppself, te_obj_st* prval)
{
	CHECK_PTYPE;
	if (prval->ty == &_te_int_ty)
	{
		(*(te_int_st**)ppself)->val *= ((te_int_st*)prval)->val;
		return *ppself;
	}
	return te_seterr("Invalid l-value type '%s' for integer multiplication", prval->ty->name);
}

te_obj_st* te_int_idiv(te_obj_st** ppself, te_obj_st* prval)
{
	CHECK_PTYPE;
	if (prval->ty == &_te_int_ty)
	{
		(*(te_int_st**)ppself)->val /= ((te_int_st*)prval)->val;
		return *ppself;
	}
	return te_seterr("Invalid l-value type '%s' for integer division", prval->ty->name);
}

te_obj_st* te_int_imod(te_obj_st** ppself, te_obj_st* prval)
{
	CHECK_PTYPE;
	if (prval->ty == &_te_int_ty)
	{
		(*(te_int_st**)ppself)->val %= ((te_int_st*)prval)->val;
		return *ppself;
	}
	return te_seterr("Invalid l-value type '%s' for integer modulo", prval->ty->name);
}

int64_t ipow(int64_t base, uint8_t exp)
{
	/*
	* Implementation by github user: orlp
	* https://gist.github.com/orlp/3551590
	*/
	static const uint8_t highest_bit_set[] =
	{
		0, 1, 2, 2, 3, 3, 3, 3,
		4, 4, 4, 4, 4, 4, 4, 4,
		5, 5, 5, 5, 5, 5, 5, 5,
		5, 5, 5, 5, 5, 5, 5, 5,
		6, 6, 6, 6, 6, 6, 6, 6,
		6, 6, 6, 6, 6, 6, 6, 6,
		6, 6, 6, 6, 6, 6, 6, 6,
		6, 6, 6, 6, 6, 6, 6, 255, // anything past 63 is a guaranteed overflow with base > 1
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
	};

	int64_t result = 1;

	switch (highest_bit_set[exp])
	{
	case 255: // we use 255 as an overflow marker and return 0 on overflow/underflow
		if (base == 1)
			return 1;
		if (base == -1)
			return 1 - 2 * (exp & 1);
		return 0;
	case 6:
		if (exp & 1) result *= base;
		exp >>= 1;
		base *= base;
	case 5:
		if (exp & 1) result *= base;
		exp >>= 1;
		base *= base;
	case 4:
		if (exp & 1) result *= base;
		exp >>= 1;
		base *= base;
	case 3:
		if (exp & 1) result *= base;
		exp >>= 1;
		base *= base;
	case 2:
		if (exp & 1) result *= base;
		exp >>= 1;
		base *= base;
	case 1:
		if (exp & 1) result *= base;
	default:
		return result;
	}
}

te_obj_st* te_int_iexp(te_obj_st** ppself, te_obj_st* prval)
{
	CHECK_PTYPE;
	if (prval->ty == &_te_int_ty)
	{
		if (((te_int_st*)prval)->val > 0)
			(*(te_int_st**)ppself)->val = ipow((*(te_int_st**)ppself)->val, ((te_int_st*)prval)->val);
		else if (((te_int_st*)prval)->val < 0)
			;  // TODO: Implementation
		else
			(*(te_int_st**)ppself)->val = 1;  // yes, 0^0 will be 1
		return *ppself;
	}
	return te_seterr("Invalid l-value type '%s' for integer modulo", prval->ty->name);
}

te_obj_st* te_int_add(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
}

te_obj_st* te_int_sub(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
}

te_obj_st* te_int_mul(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
}

te_obj_st* te_int_div(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
}

te_obj_st* te_int_mod(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
}

te_obj_st* te_int_eq(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
}

te_obj_st* te_int_ne(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
}

te_obj_st* te_int_lt(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
}

te_obj_st* te_int_gt(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
}

te_obj_st* te_int_le(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
}

te_obj_st* te_int_ge(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
}
