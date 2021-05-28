#define TEDLANG_SRC

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <tedlang/core/obj.h>
#include <tedlang/core/eval.h>
#include <tedlang/builtin/bool.h>
#include <tedlang/builtin/char.h>
#include <tedlang/builtin/int.h>
#include <tedlang/builtin/str.h>
#include <tedlang/util/string.h>

te_type_st _te_str_ty = {
	.name = "str",
	.objsize = sizeof(te_str_st),
	.ty_new = te_str_new,
	.ty_del = te_str_del,
	.ty_repr = te_str_repr,
	.ty_start = te_str_start,
	.ty_next = te_str_next,
	.ty_bool = te_str_bool,
	.ty_idx = te_str_idx,
	.ty_iadd = te_str_iadd,
	.ty_add = te_str_add,
	.ty_eq = te_str_eq,
	.ty_ne = te_str_ne
};

#define self (*(te_str_st*)pself)
#define this ((te_str_st*)*ppself) 
#define rval (*(te_str_st*)prval)
#define CHECK_TYPE_RET(ret) if (pself->ty != &_te_str_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_TYPE CHECK_TYPE_RET(NULL)
#define CHECK_PTYPE_RET(ret) if ((*ppself)->ty != &_te_str_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_PTYPE CHECK_PTYPE_RET(NULL)

te_obj_st* te_str_new()
{
	te_str_st* pstr = (te_str_st*)malloc(sizeof(te_str_st));
	if (!pstr)
		return te_seterr("Out of memory");

	te_obj_new(pstr, &_te_str_ty);
	pstr->val = NULL;
	return (te_obj_st*)pstr;
}

void te_str_del(te_obj_st* pself)
{
	CHECK_TYPE_RET();
	te_obj_del(pself);
	if (!pself->is_valid)
		return;

	if (self.val)
		free(self.val);
}

const char* te_str_repr(te_obj_st* pself)
{
	CHECK_TYPE;
	static const char* empty_str = "";

	te_str_st* pstr = pself;
	if (self.val)
		return self.val;
	return empty_str;
}

te_obj_st* te_str_start(te_obj_st* pself)
{
	CHECK_TYPE;
	return te_int_new();
}

te_obj_st* te_str_next(te_obj_st* pself, te_obj_st* pitctx)
{
	CHECK_TYPE;
	if (!self.val[((te_int_st*)pitctx)->val])
		return NULL;

	te_char_st* pchar = (te_char_st*)te_char_new();
	if (te_haserr())
		return NULL;

	pchar->ch = self.val[((te_int_st*)pitctx)->val];
	return pchar;
}

bool te_str_bool(te_obj_st* pself)
{
	CHECK_TYPE_RET(false);
	if (self.val)
		return true;
	return false;
}

te_obj_st* te_str_idx(te_obj_st* pself, te_obj_st* pidx)
{
	CHECK_TYPE;
	int64_t idx = te_int(pidx);
	if (te_haserr())
		return NULL;
	if (idx < 0 || te_strlen(self.val) <= idx)
		return te_seterr("String index out of range");

	te_char_st* pchar = (te_char_st*)te_char_new();
	if (te_haserr())
		return NULL;

	pchar->ch = self.val[idx];
	return pchar;
}

te_obj_st* te_str_iadd(te_obj_st** ppself, te_obj_st* prval)
{
	CHECK_PTYPE;
	if (prval->ty == &_te_str_ty)
	{
		size_t self_sz = te_strlen(this->val);
		size_t rval_sz = te_strlen(rval.val);
		char* ptmp = (char*)realloc(this->val, sizeof(char) * (self_sz + rval_sz + 1));
		if (!ptmp)
			return te_seterr("Out of memory");
		memcpy(ptmp + self_sz, rval.val, rval_sz);
		ptmp[self_sz + rval_sz] = '\0';
		this->val = ptmp;
		return this;
	}
	if (prval->ty == &_te_str_ty)
	{
		size_t self_sz = te_strlen(this->val);
		char* ptmp = (char*)realloc(this->val, sizeof(char) * (self_sz + 2));
		if (!ptmp)
			return te_seterr("Out of memory");
		*(uint16_t*)(ptmp + self_sz) = (((te_char_st*)prval)->ch << 8);  // ("%c\0", ch)
		this->val = ptmp;
		return this;
	}
	return te_seterr("Unable to concatenate string with type %s", prval->ty->name);
}

te_obj_st* te_str_add(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	if (prval->ty == &_te_str_ty)
	{
		te_str_st* pstr = (te_str_st*)te_str_new();
		if (te_haserr())
			return NULL;

		size_t self_sz = te_strlen(self.val);
		size_t rval_sz = te_strlen(rval.val);
		pstr->val = (char*)malloc(sizeof(char) * (self_sz + rval_sz + 1));
		if (!pstr->val)
		{
			te_decref(pstr);
			return te_seterr("Out of memory");
		}

		memcpy(pstr->val          , self.val, self_sz);
		memcpy(pstr->val + self_sz, rval.val, rval_sz);
		pstr->val[self_sz + rval_sz] = '\0';
		return pstr;
	}
	if (prval->ty == &_te_char_ty)
	{
		te_str_st* pstr = (te_str_st*)te_str_new();
		if (te_haserr())
			return NULL;

		size_t self_sz = te_strlen(self.val);
		pstr->val = (char*)malloc(sizeof(char) * (self_sz + 2));
		if (!pstr->val)
		{
			te_decref(pstr);
			return te_seterr("Out of memory");
		}

		memcpy(pstr->val, self.val, self_sz);
		*(uint16_t*)(pstr->val + self_sz) = (((te_char_st*)prval)->ch << 8);  // ("%c\0", ch)
		return pstr;
	}
	return te_seterr("Unable to concatenate string with type %s", prval->ty->name);
}

te_obj_st* te_str_eq(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	
	te_bool_st* pbool = (te_bool_st*)te_bool_new();  // defaults to false
	if (te_haserr())
		return NULL;

	if (prval->ty != &_te_str_ty)
		return pbool;
	size_t sz = te_strlen(self.val);
	if (te_strlen(rval.val) != sz)
		return pbool;
	if (memcmp(self.val, rval.val, sz))
		return pbool;
	pbool->val = true;
	return pbool;
}

te_obj_st* te_str_ne(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;

	te_bool_st* pbool = (te_bool_st*)te_bool_new();
	if (te_haserr())
		return NULL;
	pbool->val = true;

	if (prval->ty != &_te_str_ty)
		return pbool;
	size_t sz = te_strlen(self.val);
	if (te_strlen(rval.val) != sz)
		return pbool;
	if (memcmp(self.val, rval.val, sz))
		return pbool;
	pbool->val = false;
	return pbool;
}
