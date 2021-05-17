#define TEDLANG_SRC

#include <stdlib.h>

#include <tedlang/core/obj.h>
#include <tedlang/core/eval.h>
#include <tedlang/core/obj.h>
#include <tedlang/builtin/arr.h>

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
	.ty_ne = te_arr_ne,
};

#define self (*(te_arr_st*)pself)
#define CHECK_TYPE_RET(ret) if (pself->ty != &_te_arr_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_TYPE CHECK_TYPE_RET(NULL)
#define CHECK_PTYPE_RET(ret) if ((*ppself)->ty != &_te_arr_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_PTYPE CHECK_PTYPE_RET(NULL)

te_obj_st* te_arr_new()
{
	te_arr_st* nparr = (te_arr_st*)malloc(sizeof(te_arr_st));
	if (!nparr)
		return te_seterr("Out of memory");
	nparr->ppelems = NULL;
	nparr->length = 0;
	nparr->_mem_sz = 0;
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
