#define TEDLANG_SRC

#include <stdlib.h>
#include <stdbool.h>

#include <tedlang/core/obj.h>
#include <tedlang/core/eval.h>
#include <tedlang/builtin/bool.h>
#include <tedlang/builtin/int.h>
#include <tedlang/builtin/arr.h>
#include <tedlang/util/string.h>

te_type_st _te_arr_ty = {
	.name = "array",
	.objsize = sizeof(te_arr_st),
	.ty_new = te_arr_new,
	.ty_del = te_arr_del,
	.ty_bool = te_arr_bool,
	.ty_repr = te_arr_repr,
	.ty_start = te_arr_start,
	.ty_next = te_arr_next,
	.ty_iadd = te_arr_iadd,
	.ty_add = te_arr_add,
	.ty_eq = te_arr_eq,
	.ty_ne = te_arr_ne
};

#define self (*(te_arr_st*)pself)
#define rval (*(te_arr_st*)prval)
#define CHECK_TYPE_RET(ret) if (pself->ty != &_te_arr_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_TYPE CHECK_TYPE_RET(NULL)
#define CHECK_PTYPE_RET(ret) if ((*ppself)->ty != &_te_arr_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_PTYPE CHECK_PTYPE_RET(NULL)

te_obj_st* te_arr_new()
{
	te_arr_st* parr = (te_arr_st*)malloc(sizeof(te_arr_st));
	if (!parr)
		return te_seterr("Out of memory");

	te_obj_new(parr, &_te_arr_ty);
	parr->ppelems = NULL;
	parr->length = 0;
	parr->_mem_sz = 0;
	return (te_obj_st*)parr;
}

void te_arr_del(te_obj_st* pself)
{
	CHECK_TYPE_RET();
	te_obj_del(pself);
	if (!pself->is_valid)
		return;

	if (self.ppelems)
	{
		for (te_obj_st** ppelem = self.ppelems; ppelem < self.ppelems + self.length; ppelem++)
			te_decref(*ppelem);
		free(self.ppelems);
	}
}

bool te_arr_bool(te_obj_st* pself)
{
	CHECK_TYPE;
	return self.length;
}

const char* te_arr_repr(te_obj_st* pself)
{
	CHECK_TYPE;
	static char* pbuf = NULL;
	static size_t bufsz = 0;
	static bool in_repr = false;

	if (!pbuf)
	{
		pbuf = (char*)malloc(sizeof(char) * 128);
		if (!pbuf)
			return te_seterr("Out of memory");
		bufsz = 128;
		pbuf[0] = '[';
	}

	if (in_repr)
	{
		memcpy(pbuf + 1, "...]", sizeof("...]") / sizeof(char));
		return pbuf;
	}

	in_repr = true;

#define RESIZE_PBUF							    \
	while (retsz + offset >= bufsz - 1) {	    \
		size_t nsz = bufsz * 2;				    \
		char* ptmp = realloc(pbuf, nsz);	    \
		if (!ptmp) {						    \
			in_repr = false;				    \
			return te_seterr("Out of memory");} \
		pbuf = ptmp;						    \
		bufsz = nsz;						    \
	}

	if (!(self.length))
	{
		*(uint16_t*)(pbuf + 1) = 0x5D00;  // "]\0"
		in_repr = false;
		return pbuf;
	}

	size_t offset = 1;
	const char* pelemret = self.ppelems[0]->ty->ty_repr(self.ppelems[0]);
	size_t retsz = te_strlen(pelemret);
	RESIZE_PBUF;
	memcpy(pbuf + offset, pelemret, retsz);
	
	for (size_t i = 1; i < self.length; i++)
	{
		offset += retsz;
		retsz = 2;
		RESIZE_PBUF;
		*(uint16_t*)(pbuf + offset + 1) = 0x2C20;  // ", "

		pelemret = self.ppelems[i]->ty->ty_repr(self.ppelems[i]);
		offset += retsz;
		retsz = te_strlen(pelemret);
		RESIZE_PBUF;
		memcpy(pbuf + offset, pelemret, retsz);
	}

	offset += retsz;
	retsz = 2;
	RESIZE_PBUF;
	memcpy(pbuf + offset, "]", 2);
	in_repr = false;
	return pbuf;

#undef RESIZE_PBUF
}

te_obj_st* te_arr_start(te_obj_st* pself)
{
	CHECK_TYPE;
	return te_int_new();
}

te_obj_st* te_arr_next(te_obj_st* pself, te_obj_st* piterctx)
{
	if (((te_int_st*)piterctx)->val == self.length)
		return NULL;
	return self.ppelems[((te_int_st*)piterctx)->val++];
}

te_obj_st* te_arr_iadd(te_obj_st** ppself, te_obj_st* prval)
{
	CHECK_PTYPE;
	if (prval->ty != &_te_arr_ty)
		return te_seterr("Unable to concatenate array with type %s", prval->ty->name);

	// Increasing buffer if needed
	if (((te_arr_st*)*ppself)->_mem_sz < rval.length + ((te_arr_st*)*ppself)->length)
	{
		size_t nsz = rval.length + ((te_arr_st*)*ppself)->length;
		te_obj_st** pptmp = (te_obj_st**)realloc(((te_arr_st*)*ppself)->ppelems, sizeof(te_obj_st*) * nsz);  // Defaults to malloc for NULL ppelems
		if (!pptmp)
			return te_seterr("Out of memory");
		((te_arr_st*)*ppself)->ppelems = pptmp;
		((te_arr_st*)*ppself)->_mem_sz = nsz;
	}

	// Copying the element references
	for (size_t i = 0; i < rval.length; i++)
	{
		te_incref(rval.ppelems[i]);
		((te_arr_st*)*ppself)->ppelems[((te_arr_st*)*ppself)->length + i] = rval.ppelems[i];
	}

	return *ppself;
}

te_obj_st* te_arr_add(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	if (prval->ty != &_te_arr_ty)
		return te_seterr("Unable to concatenate array with type %s", prval->ty->name);

	te_arr_st* nparr = (te_arr_st*)te_arr_new();
	nparr->ppelems = (te_obj_st**)malloc(sizeof(te_obj_st*) * (self.length + rval.length));
	if (!nparr->ppelems)
	{
		te_decref(nparr);
		return te_seterr("Out of memory");
	}

	// Copying the elements from the first array
	for (size_t i = 0; i < self.length; i++)
	{
		te_incref(self.ppelems[i]);
		nparr->ppelems[i] = self.ppelems[i];
	}
	// Copying the elements from the second array
	for (size_t i = 0; i < rval.length; i++)
	{
		te_incref(rval.ppelems[i]);
		nparr->ppelems[self.length + i] = rval.ppelems[i];
	}

	return (te_obj_st*)nparr;
}

te_obj_st* te_arr_eq(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;
	if (prval->ty != &_te_arr_ty)
		return te_seterr("Unable to concatenate array with type %s", prval->ty->name);

	te_bool_st* pbool = (te_bool_st*)te_bool_new();  // initializes to false
	if (te_haserr())
		return NULL;

	if (pself == prval)
	{
		pbool->val = true;
		return pbool;
	}

	if (self.length != rval.length)
		return pbool;
	for (size_t i = 0; i < self.length; i++)
	{
		te_obj_st* peq = self.ppelems[i]->ty->ty_eq(self.ppelems[i], rval.ppelems[i]);
		if (te_haserr())
		{
			te_decref(pbool);
			return NULL;
		}

		bool result = peq->ty->ty_bool(peq);
		te_decref(peq);
		if (!result)
			return pbool;
	}

	pbool->val = true;
	return pbool;
}

te_obj_st* te_arr_ne(te_obj_st* pself, te_obj_st* prval)
{
	CHECK_TYPE;

	te_bool_st* pbool = (te_bool_st*)te_bool_new();  // initializes to false
	if (te_haserr())
		return NULL;
	pbool->val = true;

	if (prval->ty != &_te_arr_ty)
		return pbool;
	if (self.length != rval.length)
		return pbool;
	for (size_t i = 0; i < self.length; i++)
	{
		te_obj_st* pne = self.ppelems[i]->ty->ty_ne(self.ppelems[i], rval.ppelems[i]);
		if (te_haserr())
		{
			te_decref(pbool);
			return NULL;
		}

		bool result = pne->ty->ty_bool(pne);
		te_decref(pne);
		if (result)
			return pbool;
	}

	pbool->val = false;
	return pbool;
}
