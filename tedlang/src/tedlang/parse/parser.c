#define TEDLANG_SRC

#include <tedlang/parse/parser.h>
#include <tedlang/core/eval.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

void __te_ast_ty_new(te_ast_st* pself, te_ast_et ty)
{
	pself->ast_ty = ty;
	pself->lin_beg = 0;
	pself->lin_end = 0;
	pself->col_beg = 0;
	pself->col_end = 0;
}

void _te_ast_del(te_ast_st* pself)
{
	switch (pself->ast_ty)
	{
	case AST_EMPTY:     
		break;
	case AST_VAR:
		_te_ast_var_del(pself);
		break;
	case AST_NULL:
		_te_ast_null_del(pself);
		break;
	case AST_BOOL:
		_te_ast_bool_del(pself);
		break;
	case AST_INT:
		_te_ast_int_del(pself);
		break;
	case AST_FLOAT:
		_te_ast_float_del(pself);
		break;
	case AST_STR:
		_te_ast_str_del(pself);
		break;
	case AST_ARR:
		_te_ast_arr_del(pself);
		break;
	case AST_SEQ:
		_te_ast_seq_del(pself);
		break;
	case AST_FOR:
		_te_ast_for_del(pself);
		break;
	case AST_WHILE:
		_te_ast_while_del(pself);
		break;
	case AST_BREAK:
		_te_ast_break_del(pself);
		break;
	case AST_CONTINUE:

		_te_ast_continue_del(pself);
		break;
	case AST_BRANCH:
		_te_ast_branch_del(pself);
		break;
	case AST_RETURN:
		_te_ast_return_del(pself);
		break;
	case AST_CALL:
		_te_ast_call_del(pself);
		break;
	case AST_FN:
		_te_ast_fn_del(pself);
		break;
	case AST_IMP:
		_te_ast_imp_del(pself);
		break;
	case AST_MODULE:
		_te_ast_module_del(pself);
		break;
	case AST_ASSIGN:
		_te_ast_assign_del(pself);
		break;
	case AST_IADD:
		_te_ast_iadd_del(pself);
		break;
	case AST_ISUB:
		_te_ast_isub_del(pself);
		break;
	case AST_IMUL:
		_te_ast_imul_del(pself);
		break;
	case AST_IDIV:
		_te_ast_idiv_del(pself);
		break;
	case AST_IMOD:
		_te_ast_imod_del(pself);
		break;
	case AST_IEXP:
		_te_ast_iexp_del(pself);
		break;
	case AST_EQ:
		_te_ast_eq_del(pself);
		break;
	case AST_NE:
		_te_ast_ne_del(pself);
		break;
	case AST_LT:
		_te_ast_lt_del(pself);
		break;
	case AST_GT:
		_te_ast_gt_del(pself);
		break;
	case AST_LE:
		_te_ast_le_del(pself);
		break;
	case AST_GE:
		_te_ast_ge_del(pself);
		break;
	case AST_AND:
		_te_ast_and_del(pself);
		break;
	case AST_OR:
		_te_ast_or_del(pself);
		break;
	case AST_ADD:
		_te_ast_add_del(pself);
		break;
	case AST_SUB:
		_te_ast_sub_del(pself);
		break;
	case AST_MUL:
		_te_ast_mul_del(pself);
		break;
	case AST_DIV:
		_te_ast_div_del(pself);
		break;
	case AST_MOD:
		_te_ast_mod_del(pself);
		break;
	case AST_EXP:
		_te_ast_exp_del(pself);
		break;
	case AST_IDX:
		_te_ast_idx_del(pself);
		break;
	default:
		assert(false);
		return te_seterr("Invalid AST Node");  // tmp
	}
}

void _te_ast_var_new(te_ast_var_st* pself)
{
	pself->super.ast_ty = AST_VAR;
	pself->name = NULL;
}
void _te_ast_var_del(te_ast_var_st* pself)
{
	assert(pself->super.ast_ty == AST_VAR);

	if (pself->name)
		free(pself->name);
}

void _te_ast_bool_new(te_ast_bool_st* pself)
{
	pself->super.ast_ty = AST_BOOL;
	pself->val = false;
}

void _te_ast_int_new(te_ast_int_st* pself)
{
	pself->super.ast_ty = AST_INT;
	pself->val = 0;
}

void _te_ast_float_new(te_ast_float_st* pself)
{
	pself->super.ast_ty = AST_FLOAT;
	pself->val = 1.0;
}

void _te_ast_str_new(te_ast_str_st* pself)
{
	pself->super.ast_ty = AST_STR;
	pself->val = NULL;
}
void _te_ast_str_del(te_ast_str_st* pself)
{
	assert(pself->super.ast_ty == AST_STR);

	if (pself->val)
		free(pself->val);
}

int _te_ast_arr_new(te_ast_arr_st* pself, size_t sz)
{
	pself->super.ast_ty = AST_ARR;
	pself->ppelems = (te_ast_st**)malloc(sizeof(te_ast_st*) * sz);
	if (!pself->ppelems)
	{
#ifdef _DEBUG
		pself->_mem_sz = 0;
#endif
		return -1;
	}
	pself->length = 0;
	pself->_mem_sz = sz;
	return 0;
}
void _te_ast_arr_del(te_ast_arr_st* pself)
{
	assert(pself->super.ast_ty == AST_ARR);
	assert(pself->_mem_sz);

	for (te_ast_st** pe = pself->ppelems; pe < pself->ppelems + pself->length; pe++)
	{
		_te_ast_del(*pe);
		free(*pe);
	}
	free(pself->ppelems);
}
int _te_ast_arr_append(te_ast_arr_st* pself, te_ast_st* pelem)
{
	assert(pself->super.ast_ty == AST_ARR);
	assert(pself->_mem_sz);
	assert(pself->ppelems);

	// resize pself->ppnds if needed
	if (pself->length == pself->_mem_sz)
	{
		// doubling strategy
		pself->_mem_sz *= 2;
		void* n_data = realloc(pself->ppelems, sizeof(te_ast_st*) * pself->_mem_sz);
		if (!n_data)
		{
			pself->_mem_sz /= 2;
			return -1;
		}
		pself->ppelems = (te_ast_st**)n_data;
	}

	pself->ppelems[pself->length++] = pelem;
	return 0;
}

int _te_ast_seq_new(te_ast_seq_st* pself, size_t sz)
{
	pself->super.ast_ty = AST_SEQ;
	pself->ppnds = (te_ast_st**)malloc(sizeof(te_ast_st*) * sz);
	if (!pself->ppnds)
	{
#ifdef _DEBUG
		pself->_mem_sz = 0;
#endif
		return -1;
	}
	pself->expr_num = 0;
	pself->_mem_sz = sz;
	return 0;
}
void _te_ast_seq_del(te_ast_seq_st* pself)
{
	assert(pself->super.ast_ty == AST_SEQ);
	assert(pself->_mem_sz);

	for (te_ast_st** pe = pself->ppnds; pe < pself->ppnds + pself->expr_num; pe++)
	{
		_te_ast_del(*pe);
		free(*pe);
	}
	free(pself->ppnds);
}
int _te_ast_seq_append(te_ast_seq_st* pself, te_ast_st* pexpr)
{
	assert(pself->super.ast_ty == AST_SEQ);
	assert(pself->_mem_sz);

	// resize pself->ppnds if needed
	if (pself->expr_num == pself->_mem_sz)
	{
		// doubling strategy
		pself->_mem_sz *= 2;
		void* n_data = realloc(pself->ppnds, sizeof(te_ast_st*) * pself->_mem_sz);
		if (!n_data)
		{
			pself->_mem_sz /= 2;
			return -1;
		}
		pself->ppnds = (te_ast_st**)n_data;
	}

	pself->ppnds[pself->expr_num++] = pexpr;
	return 0;
}

void _te_ast_for_new(te_ast_for_st* pself)
{
	pself->super.ast_ty = AST_FOR;
	pself->body = NULL;
	pself->iter = NULL;
	pself->it = NULL;
}
void _te_ast_for_del(te_ast_for_st* pself)
{
	assert(pself->super.ast_ty == AST_FOR);

	if (pself->body)
	{
		_te_ast_del(pself->body);
		free(pself->body);
	}
	if (pself->iter)
	{
		_te_ast_del(pself->iter);
		free(pself->iter);
	}
	if (pself->it)
		free(pself->it);
}

void _te_ast_while_new(te_ast_while_st* pself)
{
	pself->super.ast_ty = AST_WHILE;
	pself->body = NULL;
	pself->cond = NULL;
}
void _te_ast_while_del(te_ast_while_st* pself)
{
	assert(pself->super.ast_ty == AST_WHILE);

	if (pself->body)
	{
		_te_ast_del(pself->body);
		free(pself->body);
	}
	if (pself->cond)
	{
		_te_ast_del(pself->cond);
		free(pself->cond);
	}
}

void _te_ast_branch_new(te_ast_branch_st* pself)
{
	pself->super.ast_ty = AST_BRANCH;
	pself->if_body = NULL;
	pself->else_body = NULL;
	pself->cond = NULL;
}
void _te_ast_branch_del(te_ast_branch_st* pself)
{
	assert(pself->super.ast_ty == AST_BRANCH);

	if (pself->if_body)
	{
		_te_ast_del(pself->if_body);
		free(pself->if_body);
	}
	if (pself->else_body)
	{
		_te_ast_del(pself->else_body);
		free(pself->else_body);
	}
	if (pself->cond)
	{
		_te_ast_del(pself->cond);
		free(pself->cond);
	}
}

void _te_ast_return_new(te_ast_return_st* pself)
{
	pself->super.ast_ty = AST_RETURN;
	pself->ret = NULL;
}
void _te_ast_return_del(te_ast_return_st* pself)
{
	assert(pself->super.ast_ty == AST_RETURN);

	if (pself->ret)
	{
		_te_ast_del(pself->ret);
		free(pself->ret);
	}
}

int _te_ast_call_new(te_ast_call_st* pself, size_t sz)
{
	pself->super.ast_ty = AST_CALL;
	pself->ppargv = (te_ast_st**)malloc(sizeof(te_ast_st*) * sz);
	if (!pself->ppargv)
	{
#ifdef _DEBUG
		pself->_mem_sz = 0;
#endif
		return -1;
	}
	pself->argc = 0;
	pself->_mem_sz = sz;
	pself->pfn = NULL;
	return 0;
}
void _te_ast_call_del(te_ast_call_st* pself)
{
	assert(pself->super.ast_ty == AST_CALL);
	assert(pself->_mem_sz);

	for (te_ast_st** pe = pself->ppargv; pe < pself->ppargv + pself->argc; pe++)
	{
		_te_ast_del(*pe);
		free(*pe);
	}
	free(pself->ppargv);
	if (pself->pfn)
	{
		_te_ast_del(pself->pfn);
		free(pself->pfn);
	}
}
int _te_ast_call_append(te_ast_call_st* pself, te_ast_st* parg)
{
	assert(pself->super.ast_ty == AST_CALL);
	assert(pself->_mem_sz);

	// resize pself->ppnds if needed
	if (pself->argc == pself->_mem_sz)
	{
		// doubling strategy
		pself->_mem_sz *= 2;
		void* n_data = realloc(pself->ppargv, sizeof(te_ast_st*) * pself->_mem_sz);
		if (!n_data)
		{
			pself->_mem_sz /= 2;
			return -1;
		}
		pself->ppargv = (te_ast_st**)n_data;
	}

	pself->ppargv[pself->argc++] = parg;
	return 0;
}

int _te_ast_fn_new(te_ast_fn_st* pself, size_t sz)
{
	pself->super.ast_ty = AST_FN;
	pself->ppargv = (char**)malloc(sizeof(char*) * sz);
	if (!pself->ppargv)
	{
#ifdef _DEBUG
		pself->_mem_sz = 0;
#endif
		return -1;
	}

	pself->pbody = NULL;
	pself->argc = 0;
	pself->_mem_sz = sz;
	pself->name = NULL;
	return 0;
}
void _te_ast_fn_del(te_ast_fn_st* pself)
{
	assert(pself->super.ast_ty == AST_FN);
	assert(pself->_mem_sz);

	if (pself->ppargv)
	{
		for (char** pe = pself->ppargv; pe < pself->ppargv + pself->argc; pe++)
			if (*pe)
				free(*pe);
		free(pself->ppargv);
	}
	if (pself->pbody)
	{
		_te_ast_del(pself->pbody);
		free(pself->pbody);
	}
	if (pself->name)
		free(pself->name);
}
int _te_ast_fn_add_arg(te_ast_fn_st* pself, char* arg)
{
	assert(pself->super.ast_ty == AST_FN);
	assert(pself->_mem_sz);

	if (pself->argc == pself->_mem_sz)
	{
		pself->_mem_sz *= 2;
		void* n_data = realloc(pself->ppargv, sizeof(te_ast_st*) * pself->_mem_sz);
		if (!n_data)
		{
			pself->_mem_sz /= 2;
			return -1;
		}
		pself->ppargv = (te_ast_st**)n_data;
	}

	pself->ppargv[pself->argc++] = arg;
	return 0;
}

int _te_ast_imp_new(te_ast_imp_st* pself, size_t sz)
{
	pself->super.ast_ty = AST_IMP;
	pself->imp_pth = (char**)malloc(sizeof(char*) * sz);
	if (!pself->imp_pth)
	{
#ifdef _DEBUG
		pself->_mem_sz = 0;
#endif
		return -1;
	}

	pself->length = 0;
	pself->_mem_sz = sz;
	return 0;
}
void _te_ast_imp_del(te_ast_imp_st* pself)
{
	assert(pself->super.ast_ty == AST_IMP);
	assert(pself->_mem_sz);
	
	for (char** pe = pself->imp_pth; pe < pself->imp_pth + pself->length; pe++)
		free(*pe);
	free(pself->imp_pth);
}
int _te_ast_imp_append(te_ast_imp_st* pself, char* imp_elem)
{
	assert(pself->super.ast_ty == AST_IMP);
	assert(pself->_mem_sz);

	if (pself->length == pself->_mem_sz)
	{
		pself->_mem_sz *= 2;
		void* n_data = realloc(pself->imp_pth, sizeof(char*) * pself->_mem_sz);
		if (!n_data)
		{
			pself->_mem_sz /= 2;
			return -1;
		}
		pself->imp_pth = (char**)n_data;
	}

	pself->imp_pth[pself->length++] = imp_elem;
	return 0;
}

int _te_ast_module_new(te_ast_module_st* pself, size_t sz)
{
	pself->super.ast_ty = AST_MODULE;
	pself->ppelems = (te_ast_st**)malloc(sizeof(te_ast_st*) * sz);
	if (!pself->ppelems)
	{
#ifdef _DEBUG
		pself->_mem_sz = 0;
#endif
		return -1;
	}

	pself->elem_num = 0;
	pself->_mem_sz = sz;
	return 0;
}
void _te_ast_module_del(te_ast_module_st* pself)
{
	assert(pself->super.ast_ty == AST_MODULE);
	assert(pself->_mem_sz);

	for (te_ast_st** pe = pself->ppelems; pe < pself->ppelems + pself->elem_num; pe++)
	{
		_te_ast_del(*pe);
		free(*pe);
	}
	free(pself->ppelems);
}
int _te_ast_module_append(te_ast_module_st* pself, te_ast_st* pelem)
{
	assert(pself->super.ast_ty == AST_MODULE);
	assert(pself->_mem_sz);

	if (pself->elem_num == pself->_mem_sz)
	{
		pself->_mem_sz *= 2;
		void* n_data = realloc(pself->ppelems, sizeof(te_ast_st**) * pself->_mem_sz);
		if (!n_data)
		{
			pself->_mem_sz /= 2;
			return -1;
		}
		pself->ppelems = (te_ast_st**)n_data;
	}

	pself->ppelems[pself->elem_num++] = pelem;
	return 0;
}

void _te_ast_bin_new(te_ast_bin_st* pself, te_ast_et ast_ty)
{
	pself->super.ast_ty = ast_ty;
	pself->lval = NULL;
	pself->rval = NULL;
}
void _te_ast_bin_del(te_ast_bin_st* pself)
{
	if (pself->lval)
	{
		_te_ast_del(pself->lval);
		free(pself->lval);
	}
	if (pself->rval)
	{
		_te_ast_del(pself->rval);
		free(pself->rval);
	}
}

int init_bin_op(te_ast_bin_st* pbin, te_token_et tk)
{
	switch (tk)
	{
	case TK_OP_ASSIGN: _te_ast_assign_new(pbin); break;
	case TK_OP_IADD: _te_ast_iadd_new(pbin); break;
	case TK_OP_ISUB: _te_ast_isub_new(pbin); break;
	case TK_OP_IMUL: _te_ast_imul_new(pbin); break;
	case TK_OP_IDIV: _te_ast_idiv_new(pbin); break;
	case TK_OP_IMOD: _te_ast_imod_new(pbin); break;
	case TK_OP_IEXP: _te_ast_iexp_new(pbin); break;

	case TK_OP_EQ: _te_ast_eq_new(pbin); break;
	case TK_OP_NE: _te_ast_ne_new(pbin); break;
	case TK_OP_LT: _te_ast_lt_new(pbin); break;
	case TK_OP_GT: _te_ast_gt_new(pbin); break;
	case TK_OP_LE: _te_ast_le_new(pbin); break;
	case TK_OP_GE: _te_ast_ge_new(pbin); break;
		
	case TK_OP_AND: _te_ast_and_new(pbin); break;
	case TK_OP_OR: _te_ast_or_new(pbin); break;

	case TK_OP_ADD: _te_ast_add_new(pbin); break;
	case TK_OP_SUB: _te_ast_sub_new(pbin); break;
	case TK_OP_MUL: _te_ast_mul_new(pbin); break;
	case TK_OP_DIV: _te_ast_div_new(pbin); break;
	case TK_OP_MOD: _te_ast_mod_new(pbin); break;
	case TK_OP_EXP: _te_ast_exp_new(pbin); break;

	default:
		assert(false && "Found a non op as an op, check the lexer\n");
		return -2;
	}
	return 0;
}

// helper functions for parsing

// Determines the position of the final token contained within a set of brackets for a given token array
int find_brac_end(const te_tarr_st* ptarr, size_t start, te_token_et to_id, te_token_et tc_id)
{
	assert(to_id != tc_id);

	size_t bnum = 1;
	for (size_t i = start; i < ptarr->length; i++)
	{
		if (ptarr->_data[i].t_id == to_id)
			bnum++;
		else if (ptarr->_data[i].t_id == tc_id)
			if (!--bnum)
				return i;
	}

	return -2;
}

// Determines the positiion of the next token with id t_id in the given token array
int find_tk(const te_tarr_st* ptarr, size_t start, te_token_et tk)
{
	for (size_t i = start; i < ptarr->length; i++)
		if (ptarr->_data[i].t_id == tk)
			return i;

	return -2;
}

// Finds the position of the end of a 'block'
int find_block_end(const te_tarr_st* ptarr, size_t start)
{
	size_t bnum = 0;
	for (size_t i = start; i < ptarr->length; i++)
	{
		switch (ptarr->_data[i].t_id)
		{
		case TK_O_BRACE:
			bnum++;
			break;
		case TK_C_BRACE:
			bnum--;
			break;
		case TK_ENDL:
			if (!bnum)
				return i + 1;
			break;
		}
	}

	return -2;
}

// Looks through the token array until a top level ',' or ptarr->length
int find_elem_end(const te_tarr_st* ptarr, size_t idx, te_token_et tk_open, te_token_et tk_close)
{
	// token values could be passed at compile time, but idk if its worth it
	int brac = 0;
	for (; idx < ptarr->length; idx++)
	{
		brac += ptarr->_data[idx].t_id == tk_open;
		brac -= ptarr->_data[idx].t_id == tk_close;
		if (!brac && ptarr->_data[idx].t_id == TK_COMMA)
			return idx;
		else if (brac == -1)
			return idx;
	}
	return ptarr->length;
}

// finds a p0 op (if one exists) starting from the right
int find_p0_op(const te_tarr_st* ptarr, int idx)
{
	int bbrac = 0;
	int rbrac = 0;
	int sbrac = 0;

	for (; idx >= 0; idx--)
	{
		switch (ptarr->_data[idx].t_id)
		{
		case TK_C_BRACE:
			bbrac++;
			continue;
		case TK_O_BRACE:
			if (!bbrac--)
				return -1;
			continue;
		case TK_C_ROUND:
			rbrac++;
			continue;
		case TK_O_ROUND:
			if (!rbrac--)
				return -1;
			continue;
		case TK_C_SQUARE:
			rbrac++;
			continue;
		case TK_O_SQUARE:
			if (!rbrac--)
				return -1;
			continue;
		}

		if (bbrac || rbrac || sbrac)
			continue;
		if (_te_op_prec(ptarr->_data[idx].t_id) != 0)
			continue;

		return idx;
	}
	return -1;
}

// expression parsing

// Parses comma seperated array elements.  ptarr must start after the '['
int parse_expr_arr(const te_tarr_st* ptarr, te_ast_arr_st* parr)
{
	if (ptarr->length == 0)
		return 0;

	te_ast_st* pelem;
	te_tarr_st tk_slice;
	size_t idx = 0;
	int ret;

	while (idx < ptarr->length)
	{
		ret = find_elem_end(ptarr, idx, TK_O_SQUARE, TK_C_SQUARE);
		if (ret < 0)
		{
			te_seterr("Invalid array element on line %zu", ptarr->_data[idx].linenum);
			return ret;
		}
		if (ret == idx)
		{
			te_seterr("Incomplete array element on line %zu", ptarr->_data[idx].linenum);
			return -2;
		}
		_te_tarr_slice(ptarr, idx, ret, &tk_slice);
		ret = te_parse_expr(&tk_slice, &pelem);
		if (ret < 0)
			return ret;
		idx += ret + 1;
		ret = _te_ast_arr_append(parr, pelem);
		if (ret < 0)
		{
			_te_ast_del(pelem);
			te_seterr("Out of memory");
			return ret;
		}
	}

	// exit condition
	return idx - 1;
}

// Parses comma seperated function arguments.  ptarr must start after the '('
int parse_expr_call(const te_tarr_st* ptarr, te_ast_call_st* parr)
{
	if (ptarr->length == 0)
		return 0;

	te_ast_st* pelem;
	te_tarr_st tk_slice;
	int idx = 0;
	int ret;

	while (idx < ptarr->length)
	{
		ret = find_elem_end(ptarr, idx, TK_O_ROUND, TK_C_ROUND);
		if (ret < 0)
		{
			te_seterr("Invalid argument syntax on line %zu", ptarr->_data[idx].linenum);
			return ret;
		}
		if (ret == idx)
		{
			te_seterr("Incomplete call argument on line %zu", ptarr->_data[idx].linenum);
			return -2;
		}
		_te_tarr_slice(ptarr, idx, ret, &tk_slice);
		ret = te_parse_expr(&tk_slice, &pelem);
		if (ret < 0)
			return ret;
		idx += ret + 1;
		ret = _te_ast_call_append(parr, pelem);
		if (ret < 0)
		{
			_te_ast_del(pelem);
			te_seterr("Out of memory");
			return ret;
		}
	}
	return idx - 1;
}

// Parses a leaf node for an expression
int parse_expr_leaf(const te_tarr_st* ptarr, te_ast_st** ppexpr)
{
	if (!ptarr->length)
	{
		te_ast_noexpr_st* pexpr = (te_ast_noexpr_st*)malloc(sizeof(te_ast_noexpr_st));
		if (!pexpr)
			goto OUT_OF_MEMORY;
		_te_ast_noexpr_new(pexpr);
		*ppexpr = pexpr;
		return 0;
	}

	if (ptarr->length > 1)
	{
		te_tarr_st tk_slice;
		te_ast_st* pexpr;
		int ret;
		int i;

		switch (ptarr->_data[0].t_id)
		{
		case TK_O_SQUARE:
			pexpr = (te_ast_st*)malloc(sizeof(te_ast_arr_st));
			if (!pexpr)
				goto OUT_OF_MEMORY;
			ret = _te_ast_arr_new((te_ast_arr_st*)pexpr, ptarr->length / 2);
			if (ret < 0)
			{
				free(pexpr);
				goto OUT_OF_MEMORY;
			}
			ret = find_brac_end(ptarr, 1, TK_O_SQUARE, TK_C_SQUARE);
			if (ret < 0)
			{
				_te_ast_arr_del((te_ast_arr_st*)pexpr);
				free(pexpr);
				return ret;
			}
			_te_tarr_slice(ptarr, 1, ret, &tk_slice);
			ret = parse_expr_arr(&tk_slice, (te_ast_arr_st*)pexpr);
			if (ret < 0)
			{
				_te_ast_arr_del((te_ast_arr_st*)pexpr);
				free(pexpr);
				return ret;
			}
			i = ret;
			break;

		case TK_STR_LIT:
			// Creating the string ast node
			pexpr = (te_ast_st*)malloc(sizeof(te_ast_str_st));
			if (!pexpr)
				goto OUT_OF_MEMORY;
			_te_ast_str_new((te_ast_str_st*)pexpr);
			((te_ast_str_st*)pexpr)->val = ptarr->_data[0]._data;
			ptarr->_data[0]._data = NULL;
			i = 1;
			break;

		case TK_IDN:
			pexpr = (te_ast_st*)malloc(sizeof(te_ast_var_st));
			if (!pexpr)
				goto OUT_OF_MEMORY;
			_te_ast_var_new((te_ast_var_st*)pexpr);
			((te_ast_var_st*)pexpr)->name = ptarr->_data[0]._data;
			ptarr->_data[0]._data = NULL;
			i = 1;
			break;

		default:
			fprintf(stderr, "Unexpected token on line %zu\n", ptarr->_data[0].linenum);
			return -2;
		}

		// base case
		if (ptarr->length == i)
		{
			*ppexpr = pexpr;
			return i;
		}

		for (; i < ptarr->length; i++)
		{
			switch (ptarr->_data[i].t_id)
			{
			case TK_O_ROUND:
			{
				// function call

				if (ptarr->_data[ptarr->length - 1].t_id != TK_C_ROUND)
				{
					fprintf(stderr, "Missing closing ')' on line %zu\n", ptarr->_data[i].linenum);
					_te_ast_del(pexpr);
					free(pexpr);
					return -2;
				}

				// allocating the call node
				te_ast_call_st* pcall = (te_ast_call_st*)malloc(sizeof(te_ast_call_st));
				if (!pcall)
				{
					_te_ast_del(pexpr);
					free(pexpr);
					goto OUT_OF_MEMORY;
				}
				ret = _te_ast_call_new(pcall, ptarr->length / 2);
				if (ret < 0)
				{
					free(pcall);
					_te_ast_del(pexpr);
					free(pexpr);
					goto OUT_OF_MEMORY;
				}
				pcall->pfn = pexpr;
				pexpr = pcall;

				// parsing the arguments
				_te_tarr_slice(ptarr, (size_t)++i, ptarr->length - 1, &tk_slice);
				ret = parse_expr_call(&tk_slice, pcall);
				if (ret < 0)
				{
					_te_ast_del(pexpr);
					free(pexpr);
					return ret;
				}

				// fast foward by ret
				i += ret + 1;
				break;
			}
			case TK_O_BRACE:
			{
				// indexing

				// finding the closing ']'
				ret = find_brac_end(ptarr, (size_t)i + 1, TK_O_SQUARE, TK_C_SQUARE);
				if (ret < 0)
				{
					fprintf(stderr, "Missing closing ']' on line %zu\n", ptarr->_data[i].linenum);
					_te_ast_del(pexpr);
					free(pexpr);
					return ret;
				}

				// creating the idx node
				te_ast_idx_st* pidx = (te_ast_idx_st*)malloc(sizeof(te_ast_idx_st));
				if (!pidx)
				{
					_te_ast_del(pexpr);
					free(pexpr);
					goto OUT_OF_MEMORY;
				}
				_te_ast_idx_new(pidx);
				pidx->lval = pexpr;
				pexpr = pidx;

				// parsing the idx rval
				_te_tarr_slice(ptarr, (size_t)i + 1, ret, &tk_slice);
				ret = te_parse_expr(&tk_slice, &(pidx->rval));
				if (ret < 0)
				{
					_te_ast_del(pexpr);
					free(pexpr);
					return ret;
				}

				// fast forward by ret
				i += ret;
				break;
			}
			default:
				fprintf(stderr, "Unexpected token on line %zu\n", ptarr->_data[i].linenum);
				return -2;
			}
		}

		*ppexpr = pexpr;
		return ptarr->length;
	}

	// handling single token leaf nodes
	switch (ptarr->_data[0].t_id)
	{
	case TK_NULL:
	{
		te_ast_null_st* pnull = (te_ast_null_st*)malloc(sizeof(te_ast_null_st));
		if (!pnull)
			goto OUT_OF_MEMORY;
		_te_ast_null_new(pnull);
		*ppexpr = pnull;
		return 1;
	}
	case TK_TRUE:
	{
		te_ast_bool_st* pbool = (te_ast_bool_st*)malloc(sizeof(te_ast_bool_st));
		if (!pbool)
			goto OUT_OF_MEMORY;
		_te_ast_bool_new(pbool);
		pbool->val = true;
		*ppexpr = pbool;
		return 1;
	}
	case TK_FALSE:
	{
		te_ast_bool_st* pbool = (te_ast_bool_st*)malloc(sizeof(te_ast_bool_st));
		if (!pbool)
			goto OUT_OF_MEMORY;
		_te_ast_bool_new(pbool);
		pbool->val = false;
		*ppexpr = pbool;
		return 1;
	}
	case TK_INT_LIT:
	{
		te_ast_int_st* pint = (te_ast_int_st*)malloc(sizeof(te_ast_int_st));
		if (!pint)
			goto OUT_OF_MEMORY;
		_te_ast_int_new(pint);
		pint->val = (int64_t)(ptarr->_data[0]._data);
		*ppexpr = pint;
		return 1;
	}
	case TK_FLOAT_LIT:
	{
		te_ast_float_st* pfloat = (te_ast_float_st*)malloc(sizeof(te_ast_float_st));
		if (!pfloat)
			goto OUT_OF_MEMORY;
		_te_ast_float_new(pfloat);
		pfloat->val = *(double*)(&ptarr->_data[0]._data);
		*ppexpr = pfloat;
		return 1;
	}
	case TK_STR_LIT:
	{
		te_ast_str_st* pstr = (te_ast_str_st*)malloc(sizeof(te_ast_str_st));
		if (!pstr)
			goto OUT_OF_MEMORY;
		_te_ast_str_new(pstr);
		pstr->val = ptarr->_data[0]._data;
		ptarr->_data[0]._data = NULL;
		*ppexpr = pstr;
		return 1;
	}
	case TK_IDN:
	{
		te_ast_var_st* pvar = (te_ast_var_st*)malloc(sizeof(te_ast_var_st));
		if (!pvar)
			goto OUT_OF_MEMORY;
		_te_ast_var_new(pvar);
		pvar->name = ptarr->_data[0]._data;
		ptarr->_data[0]._data = NULL;
		*ppexpr = pvar;
		return 1;
	}
	default:
		fprintf(stderr, "Unexpected token on line %zu", ptarr->_data[0].linenum);
		return -2;
	}

OUT_OF_MEMORY:
	fprintf(stderr, "Out of memory\n");
	return -1;
}

// Parses expressions with any precedence operators
int parse_expr_prec(const te_tarr_st* ptarr, te_ast_st** ppexpr, int prec);

// Parses expressions with any precedence operators.  R to L parsing => L to R evaluation
int parse_expr_rtolprec(const te_tarr_st* ptarr, te_ast_st** ppexpr, int prec)
{
	if (!ptarr->length)
	{
		te_ast_noexpr_st* pexpr = (te_ast_noexpr_st*)malloc(sizeof(te_ast_noexpr_st));
		if (!pexpr)
			goto OUT_OF_MEMORY;
		_te_ast_noexpr_new(pexpr);
		*ppexpr = pexpr;
		return 0;
	}

	te_tarr_st tk_slice;
	te_ast_st* pexpr;
	int ret;

	int bbrac = 0;
	int rbrac = 0;
	int sbrac = 0;

	int i = ptarr->length - 1;
	for (; i >= 0; i--)
	{
		switch (ptarr->_data[i].t_id)
		{
		case TK_C_BRACE:
			bbrac++;
			continue;
		case TK_O_BRACE:
			if (!bbrac--)
				goto INVALID_TOKEN;
			continue;
		case TK_C_ROUND:
			rbrac++;
			continue;
		case TK_O_ROUND:
			if (!rbrac--)
				goto INVALID_TOKEN;
			continue;
		case TK_C_SQUARE:
			rbrac++;
			continue;
		case TK_O_SQUARE:
			if (!rbrac--)
				goto INVALID_TOKEN;
			continue;
		}

		if (bbrac || rbrac || sbrac)
			continue;
		if (_te_op_prec(ptarr->_data[i].t_id) != prec)
			continue;

		// Found the split point

		if (i == 0)
			goto INVALID_TOKEN;

		te_ast_bin_st* pbin = (te_ast_bin_st*)malloc(sizeof(te_ast_bin_st));
		if (!pbin)
			goto OUT_OF_MEMORY;
		if (init_bin_op(pbin, ptarr->_data[i].t_id) < 0)
			return -2;

		_te_tarr_slice(ptarr, 0, i, &tk_slice);
		ret = parse_expr_prec(&tk_slice, &pexpr, prec);
		if (ret < 0)
		{
			_te_ast_bin_del(pbin);
			free(pbin);
			return ret;
		}
		pbin->lval = pexpr;

		_te_tarr_slice(ptarr, i + 1, ptarr->length, &tk_slice);
		ret = parse_expr_prec(&tk_slice, &pexpr, prec + 1);
		if (ret < 0)
		{
			_te_ast_bin_del(pbin);
			free(pbin);
			return ret;
		}
		pbin->rval = pexpr;
		
		*ppexpr = pbin;
		return i + 1 + ret;
	}

	// Max precedence level was reached
	if (prec == MAX_PREC)
	{
		// Checking for parentheses
		if (ptarr->_data[0].t_id == TK_O_ROUND)
		{
			i = ptarr->length - 1;
			if (ptarr->_data[i].t_id != TK_C_ROUND)
				goto INVALID_TOKEN;

			_te_tarr_slice(ptarr, 1, i, &tk_slice);
			return te_parse_expr(&tk_slice, ppexpr);
		}

		// the tokens MUST form an operator-less near leaf node
		return parse_expr_leaf(ptarr, ppexpr);
	}

	// No operators were found at the current precedence level -> inc and search again
	ret = parse_expr_prec(ptarr, &pexpr, prec + 1);
	if (ret < 0)
		return ret;
	*ppexpr = pexpr;
	return ptarr->length;

INVALID_TOKEN:
	fprintf(stderr, "Unexpected token on line %zu\n", ptarr->_data[i].linenum);
	return -2;

OUT_OF_MEMORY:
	fprintf(stderr, "Out of memory\n");
	return -1;
}

// Parses expressions with any precedence operators.  L to R parsing => R to L evaluation
int parse_expr_ltorprec(const te_tarr_st* ptarr, te_ast_st** ppexpr, int prec)
{
	if (!ptarr->length)
	{
		te_ast_noexpr_st* pexpr = (te_ast_noexpr_st*)malloc(sizeof(te_ast_noexpr_st));
		if (!pexpr)
			goto OUT_OF_MEMORY;
		_te_ast_noexpr_new(pexpr);
		*ppexpr = pexpr;
		return 0;
	}

	te_tarr_st tk_slice;
	te_ast_st* pexpr;
	int ret;

	int bbrac = 0;
	int rbrac = 0;
	int sbrac = 0;

	int i = 0;
	for (; i < ptarr->length; i++)
	{
		switch (ptarr->_data[i].t_id)
		{
		case TK_O_BRACE:
			bbrac++;
			continue;
		case TK_C_BRACE:
			if (!bbrac--)
				goto INVALID_TOKEN;
			continue;
		case TK_O_ROUND:
			rbrac++;
			continue;
		case TK_C_ROUND:
			if (!rbrac--)
				goto INVALID_TOKEN;
			continue;
		case TK_O_SQUARE:
			rbrac++;
			continue;
		case TK_C_SQUARE:
			if (!rbrac--)
				goto INVALID_TOKEN;
			continue;
		}

		if (bbrac || rbrac || sbrac)
			continue;
		if (_te_op_prec(ptarr->_data[i].t_id) != prec)
			continue;

		// Found the split point

		if (i >= ptarr->length - 1)
			goto INVALID_TOKEN;

		te_ast_bin_st* pbin = (te_ast_bin_st*)malloc(sizeof(te_ast_bin_st));
		if (!pbin)
			goto OUT_OF_MEMORY;
		if (init_bin_op(pbin, ptarr->_data[i].t_id) < 0)
			return -2;

		_te_tarr_slice(ptarr, 0, i, &tk_slice);
		ret = parse_expr_prec(&tk_slice, &pexpr, prec + 1);
		if (ret < 0)
		{
			_te_ast_bin_del(pbin);
			free(pbin);
			return ret;
		}
		pbin->lval = pexpr;

		_te_tarr_slice(ptarr, i + 1, ptarr->length, &tk_slice);
		ret = parse_expr_prec(&tk_slice, &pexpr, prec);
		if (ret < 0)
		{
			_te_ast_bin_del(pbin);
			free(pbin);
			return ret;
		}
		pbin->rval = pexpr;

		*ppexpr = pbin;
		return i + 1 + ret;
	}

	// Max precedence level was reached
	if (prec == MAX_PREC)
	{
		// Checking for parentheses
		if (ptarr->_data[0].t_id == TK_O_ROUND)
		{
			i = ptarr->length - 1;
			if (ptarr->_data[i].t_id != TK_C_ROUND)
				goto INVALID_TOKEN;

			_te_tarr_slice(ptarr, 1, i, &tk_slice);
			return te_parse_expr(&tk_slice, ppexpr);
		}

		// the tokens MUST form an operator-less near leaf node
		return parse_expr_leaf(ptarr, ppexpr);
	}

	// No operators were found at the current precedence level -> inc and search again
	ret = parse_expr_prec(ptarr, &pexpr, prec + 1);
	if (ret < 0)
		return ret;
	*ppexpr = pexpr;
	return ptarr->length;

INVALID_TOKEN:
	fprintf(stderr, "Unexpected token on line %zu", ptarr->_data[i].linenum);
	return -2;

OUT_OF_MEMORY:
	fprintf(stderr, "Out of memory\n");
	return -1;
}

int parse_expr_prec(const te_tarr_st* ptarr, te_ast_st** ppexpr, int prec)
{
	// adjust this function for added precedence levels with different evaluation orders

	if (prec) // Currently only precedence 0 uses right to left evaulation (L to R parsing)
		return parse_expr_ltorprec(ptarr, ppexpr, prec);
	return parse_expr_rtolprec(ptarr, ppexpr, prec);
}

// Parses an import expression
int parse_expr_imp(const te_tarr_st* ptarr, te_ast_imp_st* pimp)
{
	assert(ptarr->length > 0);
	assert(ptarr->_data[0].t_id == TK_IMPORT);

	if (ptarr->length < 2)
		goto INVALID_IMP;
	if (ptarr->length % 2)  // import idn [dot idn] ;
		goto INVALID_IMP;

	if (ptarr->_data[1].t_id != TK_IDN)
		goto INVALID_IMP;

	int ret;
	ret = _te_ast_imp_append(pimp, ptarr->_data[1]._data);
	if (ret < 0)
		goto OUT_OF_MEMORY;
	ptarr->_data[1]._data = NULL;

	size_t i = 2;
	while (i < ptarr->length)
	{
		if (ptarr->_data[i++].t_id != TK_DOT)
			goto INVALID_IMP;
		if (ptarr->_data[i].t_id != TK_IDN)
			goto INVALID_IMP;

		ret = _te_ast_imp_append(pimp, ptarr->_data[i]._data);
		if (ret < 0)
			goto OUT_OF_MEMORY;
		ptarr->_data[i++]._data = NULL;
	}
	return ptarr->length;

INVALID_IMP:
	fprintf(stderr, "Invalid import expression on line %zu\n", ptarr->_data[0].linenum);
	return -2;

OUT_OF_MEMORY:
	fprintf(stderr, "Out of memory\n");
	return -1;
}

// Parses an arbitrary expression
int te_parse_expr(const te_tarr_st* ptarr, te_ast_st** ppexpr)
{
	if (!ptarr->length)
	{
		te_ast_noexpr_st* pexpr = (te_ast_noexpr_st*)malloc(sizeof(te_ast_noexpr_st));
		if (!pexpr)
			goto OUT_OF_MEMORY;
		_te_ast_noexpr_new(pexpr);
		*ppexpr = pexpr;
		return 0;
	}

	// handling leaf nodes and special cases
	int ret;
	switch (ptarr->_data[0].t_id)
	{
	// special cases for expressions
	case TK_BREAK:
	{
		if (ptarr->length != 1)
		{
			fprintf(stderr, "Invalid break expression on line %zu\n", ptarr->_data[0].linenum);
			return -2;
		}
		te_ast_break_st* pbreak = (te_ast_break_st*)malloc(sizeof(te_ast_break_st));
		if (!pbreak)
			goto OUT_OF_MEMORY;
		_te_ast_break_new(pbreak);
		*ppexpr = pbreak;
		return ptarr->length;
	}
	case TK_CONTINUE:
	{
		if (ptarr->length != 1)
		{
			fprintf(stderr, "Invalid continue expression on line %zu\n", ptarr->_data[0].linenum);
			return -2;
		}
		te_ast_continue_st* pcontinue = (te_ast_continue_st*)malloc(sizeof(te_ast_continue_st));
		if (!pcontinue)
			goto OUT_OF_MEMORY;
		_te_ast_continue_new(pcontinue);
		*ppexpr = pcontinue;
		return ptarr->length;
	}
	case TK_RETURN:
	{
		te_ast_return_st* preturn = (te_ast_return_st*)malloc(sizeof(te_ast_return_st));
		if (!preturn)
			goto OUT_OF_MEMORY;
		_te_ast_return_new(preturn);
		te_tarr_st tk_slice;
		_te_tarr_slice(ptarr, 1, ptarr->length, &tk_slice);
		ret = te_parse_expr(&tk_slice, &(preturn->ret));
		if (ret < 0)
		{
			_te_ast_return_del(preturn);
			free(preturn);
			return ret;
		}
		*ppexpr = preturn;
		return ptarr->length;
	}
	case TK_IMPORT:
	{
		te_ast_imp_st* pimp = (te_ast_imp_st*)malloc(sizeof(te_ast_imp_st));
		if (!pimp)
			goto OUT_OF_MEMORY;
		ret = _te_ast_imp_new(pimp, 1);
		if (ret < 0)
		{
			free(pimp);
			goto OUT_OF_MEMORY;
		}
		ret = parse_expr_imp(ptarr, pimp);
		if (ret < 0)
		{
			_te_ast_imp_del(pimp);
			free(pimp);
			return ret;
		}
		*ppexpr = pimp;
		return ret;
	}
	default:
		return parse_expr_prec(ptarr, ppexpr, 0);
	}

OUT_OF_MEMORY:
	fprintf(stderr, "Out of memory\n");
	return -1;
}

// block parsing

// Parses a branch block, ie. if (...) ...; else ...;
int parse_branch(const te_tarr_st* ptarr, te_ast_branch_st* pbranch)
{
	assert(ptarr->length);
	assert(ptarr->_data[0].t_id == TK_IF);

	if (ptarr->length < 4)  // if ();  <- minimal if statement
	{
		fprintf(stderr, "Invalid if statement on line %zu\n", ptarr->_data[0].linenum);
		return -2;
	}

	te_ast_st* past;
	te_tarr_st tk_slice;

	size_t start;
	size_t end = 0;
	int ret;

	if (ptarr->_data[1].t_id != TK_O_ROUND)
	{
		fprintf(stderr, "Unexpected token after 'if' on line %zu\n", ptarr->_data[1].linenum);
		return -2;
	}

	// parsing the condition expression
	start = 2;
	ret = find_brac_end(ptarr, start, TK_O_ROUND, TK_C_ROUND);
	if (ret < 0)
	{
		fprintf(stderr, "Missing closing ')' after 'if' on line %zu\n", ptarr->_data[end].linenum);
		return ret;
	}
	end = ret;
	_te_tarr_slice(ptarr, start, end, &tk_slice);
	ret = te_parse_expr(&tk_slice, &past);
	if (ret < 0)
		return ret;
	pbranch->cond = past;

	// parsing the if block
	start = end + 1;
	ret = find_block_end(ptarr, start);
	if (ret < 0)
	{
		fprintf(stderr, "Unable to find the end of block after if statement on line %zu\n", ptarr->_data[end].linenum);
		return ret;
	}
	end = ret;
	_te_tarr_slice(ptarr, start, end, &tk_slice);
	ret = te_parse_block(&tk_slice, &past);
	if (ret < 0)
		return ret;
	pbranch->if_body = past;

	// parsing the (optional) else block
	start = end;
	if (start < ptarr->length && ptarr->_data[start].t_id == TK_ELSE)
	{
		// Allow for chaining via else if
		_te_tarr_slice(ptarr, start + 1, ptarr->length, &tk_slice);
		ret = te_parse_block(&tk_slice, &past);
		if (ret < 0)
			return ret;
		pbranch->else_body = past;
		end += ret;
	}
	else
		pbranch->else_body = NULL;

	return end;
}

// Parses a for loop, ie. for (... : ...) ...;
int parse_for(const te_tarr_st* ptarr, te_ast_for_st* pfor)
{
	assert(ptarr->length);
	assert(ptarr->_data[0].t_id == TK_FOR);

	if (ptarr->length < 7)  // for (_ : _);   <- minimal for loop
	{
		fprintf(stderr, "Invalid for loop on line %zu\n", ptarr->_data[0].linenum);
		return -2;
	}

	te_ast_st* past;
	te_tarr_st tk_slice;

	size_t idx;
	int ret;

	if (ptarr->_data[1].t_id != TK_O_ROUND)
	{
		fprintf(stderr, "Unexpected token after 'for' on line %zu\n", ptarr->_data[1].linenum);
		return -2;
	}
	if (ptarr->_data[2].t_id != TK_IDN)
	{
		fprintf(stderr, "Unexpected token after 'for' on line %zu\n", ptarr->_data[2].linenum);
		return -2;
	}
	// Transferring ownership of the identifier memory to pfor
	pfor->it = ptarr->_data[2]._data;
	ptarr->_data[2]._data = NULL;
	if (ptarr->_data[3].t_id != TK_ITER)
	{
		fprintf(stderr, "Unexpected token after 'for' on line %zu\n", ptarr->_data[3].linenum);
		return -2;
	}

	ret = find_brac_end(ptarr, 4, TK_O_ROUND, TK_C_ROUND);
	if (ret < 0)
	{
		fprintf(stderr, "Missing closing ')' after 'for' on line %zu\n", ptarr->_data[0].linenum);
		return ret;
	}
	idx = ret;
	_te_tarr_slice(ptarr, 4, idx, &tk_slice);
	ret = te_parse_expr(&tk_slice, &past);
	if (ret < 0)
		return ret;
	idx++;
	pfor->iter = past;
	_te_tarr_slice(ptarr, idx, ptarr->length, &tk_slice);
	ret = te_parse_block(&tk_slice, &past);
	if (ret < 0)
		return ret;
	pfor->body = past;
	return idx + ret;
}

// Parses a while loop, ie. while (...) ...;
int parse_while(const te_tarr_st* ptarr, te_ast_while_st* pwhile)
{
	assert(ptarr->length);
	assert(ptarr->_data[0].t_id == TK_WHILE);

	if (ptarr->length < 4)  // while ();   <- minimal while loop
	{
		fprintf(stderr, "Invalid while loop on line %zu\n", ptarr->_data[0].linenum);
		return -2;
	}

	if (ptarr->_data[1].t_id != TK_O_ROUND)
	{
		fprintf(stderr, "Unexpected token after 'while' on line %zu\n", ptarr->_data[1].linenum);
		return -2;
	}

	// TODO: Finish while loop parsing
	return -1;
}

// Parses a block containing a sequence of blocks, ie. {...; ...; ...};
int parse_seq(const te_tarr_st* ptarr, te_ast_seq_st* pseq)
{
	te_tarr_st tk_slice;
	te_ast_st* past;
	size_t idx = 1;
	int ret;

	while (idx < ptarr->length)
	{
		// Checking for end of sequence
		if (ptarr->_data[idx].t_id == TK_C_BRACE)
		{
			if (++idx == ptarr->length || ptarr->_data[idx].t_id != TK_ENDL)
			{
				fprintf(stderr, "Unexpected token on line %zu\n", ptarr->_data[idx - 1].linenum);
				return -2;
			}
			return idx + 1;
		}

		// Parsing the next block in the sequence
		_te_tarr_slice(ptarr, idx, ptarr->length, &tk_slice);
		ret = te_parse_block(&tk_slice, &past);
		if (ret < 0)
			return ret;
		idx += ret;
		ret = _te_ast_seq_append(pseq, past);
		if (ret < 0)
		{
			fprintf(stderr, "Out of memory\n");
			_te_ast_del(past);
			return ret;
		}
	}

	fprintf(stderr, "Invalid sequence on line %zu\n", ptarr->_data[ptarr->length - 1].linenum);
	return -2;
}

// Parses an arbitrary block
int te_parse_block(const te_tarr_st* ptarr, te_ast_st** ppblock)
{
	if (!ptarr->length)
	{
		fprintf(stderr, "Received empty code block\n");
		return -2;
	}

	int ret;
	switch (ptarr->_data[0].t_id)
	{
	case TK_IF:
	{
		te_ast_branch_st* pbranch = (te_ast_branch_st*)malloc(sizeof(te_ast_branch_st));
		if (!pbranch)
			goto OUT_OF_MEMORY;
		_te_ast_branch_new(pbranch);
		ret = parse_branch(ptarr, pbranch);
		if (ret < 0)
		{
			_te_ast_branch_del(pbranch);
			free(pbranch);
			return ret;
		}
		*ppblock = pbranch;
		return ret;
	}
	case TK_FOR:
	{
		te_ast_for_st* pfor = (te_ast_for_st*)malloc(sizeof(te_ast_for_st));
		if (!pfor)
			goto OUT_OF_MEMORY;
		_te_ast_for_new(pfor);
		ret = parse_for(ptarr, pfor);
		if (ret < 0)
		{
			_te_ast_for_del(pfor);
			free(pfor);
			return ret;
		}
		*ppblock = pfor;
		return ret;
	}
	case TK_WHILE:
	{
		te_ast_while_st* pwhile = (te_ast_while_st*)malloc(sizeof(te_ast_while_st));
		if (!pwhile)
			goto OUT_OF_MEMORY;
		_te_ast_while_new(pwhile);
		ret = parse_while(ptarr, pwhile);
		if (ret < 0)
		{
			_te_ast_while_del(pwhile);
			free(pwhile);
			return ret;
		}
		*ppblock = pwhile;
		return ret;
	}
	case TK_O_BRACE:
	{
		te_ast_seq_st* pseq = (te_ast_seq_st*)malloc(sizeof(te_ast_seq_st));
		if (!pseq)
			goto OUT_OF_MEMORY;
		_te_ast_seq_new(pseq, 5);
		ret = parse_seq(ptarr, pseq);
		if (ret < 0)
		{
			_te_ast_seq_del(pseq);
			free(pseq);
			return ret;
		}
		*ppblock = pseq;
		return ret;
	}
	}
	
	ret = find_tk(ptarr, 0, TK_ENDL);
	if (ret < 0)
	{
		fprintf(stderr, "Invalid code block on line %zu", ptarr->_data[0].linenum);
		return ret;
	}

	te_tarr_st tk_slice;
	_te_tarr_slice(ptarr, 0, ret, &tk_slice);
	ret = te_parse_expr(&tk_slice, ppblock);
	return ret + (ret >= 0);  // If successful, add the ;

OUT_OF_MEMORY:
	fprintf(stderr, "Out of memory\n");
	return -1;
}

// module parsing

// Parses an fn definition, ie. fn f(...) ...;
int parse_func(const te_tarr_st* ptarr, te_ast_fn_st* pfn)
{
	int ret;
	size_t i = 0;

	if (ptarr->length < 5) // fn f(); <- minimum function definition
		goto INVALID_FNDEF;
	if (ptarr->_data[++i].t_id != TK_IDN)
		goto INVALID_FNDEF;
	// moving the idn name into the fn node
	pfn->name = ptarr->_data[i]._data;
	ptarr->_data[i]._data = NULL;
	if (ptarr->_data[++i].t_id != TK_O_ROUND)
		goto INVALID_FNDEF;
	if (ptarr->_data[++i].t_id != TK_C_ROUND)
	{
		// parse the argument names
		if (ptarr->_data[i].t_id != TK_IDN)
			goto INVALID_FNDEF;

		// moving the first identifier into the ast node
		ret = _te_ast_fn_add_arg(pfn, ptarr->_data[i]._data);
		if (ret < 0)
			goto OUT_OF_MEMORY;
		ptarr->_data[i++]._data = NULL;
		if (ptarr->length == i)
			goto INVALID_FNDEF;

		// parsing the remaining arguments
		while (ptarr->_data[i].t_id != TK_C_ROUND)
		{
			if (ptarr->_data[i].t_id != TK_IDN)
				goto INVALID_FNDEF;
			ret = _te_ast_fn_add_arg(pfn, ptarr->_data[i]._data);
			if (ret < 0)
				goto OUT_OF_MEMORY;
			ptarr->_data[i++]._data = NULL;
			if (ptarr->length == i)
				goto INVALID_FNDEF;
			if (ptarr->_data[i++].t_id != TK_COMMA)
				goto INVALID_FNDEF;
			if (ptarr->length == i)
				goto INVALID_FNDEF;
		}
	}
	assert(ptarr->_data[i].t_id == TK_C_ROUND);

	// parse the function body
	te_tarr_st tk_slice;
	_te_tarr_slice(ptarr, ++i, ptarr->length, &tk_slice);
	ret = te_parse_block(&tk_slice, &(pfn->pbody));
	if (ret < 0)
		return ret;
	return i + ret;

INVALID_FNDEF:
	fprintf(stderr, "Invalid function declaration on line %zu\n", ptarr->_data[i].linenum);
	return -2;

OUT_OF_MEMORY:
	fprintf(stderr, "Out of memory\n");
	return -1;
}

// Returns the number of tokens read on success, -1 on error
int te_parse_module(const te_tarr_st* ptarr, te_ast_st** ppast)
{
	te_tarr_st tk_slice;
	te_ast_st* pelem;
	int ret;
	
	te_ast_module_st* pmodule = (te_ast_module_st*)malloc(sizeof(te_ast_module_st));
	if (!pmodule)
	{
		fprintf(stderr, "Out of memory\n");
		return -1;
	}
	ret = _te_ast_module_new(pmodule, 10);
	if (ret < 0)
	{
		free(pmodule);
		fprintf(stderr, "Out of memory\n");
		return -1;
	}

	size_t i = 0;
	while (i < ptarr->length)
	{
		if (ptarr->_data[i].t_id == TK_FN)
		{
			pelem = (te_ast_st*)malloc(sizeof(te_ast_fn_st));
			if (!pelem)
				goto OUT_OF_MEMORY;
			ret = _te_ast_fn_new((te_ast_fn_st*)pelem, 2);
			if (ret < 0)
			{
				free(pelem);
				goto OUT_OF_MEMORY;
			}

			_te_tarr_slice(ptarr, i, ptarr->length, &tk_slice);
			ret = parse_func(&tk_slice, (te_ast_fn_st*)pelem);
			if (ret < 0)
			{
				_te_ast_fn_del((te_ast_fn_st*)pelem);
				free(pelem);
				goto CLEANUP;
			}
		}
		else
		{
			_te_tarr_slice(ptarr, i, ptarr->length, &tk_slice);
			ret = te_parse_block(&tk_slice, &pelem);
			if (ret < 0)
				goto CLEANUP;
		}

		// Updating the index and appending the new element
		i += ret;
		ret = _te_ast_module_append(pmodule, pelem);
		if (ret < 0)
		{
			_te_ast_del(pelem);
			free(pelem);
			goto OUT_OF_MEMORY;
		}
	}
	*ppast = pmodule;
	return ptarr->length;

OUT_OF_MEMORY:
	fprintf(stderr, "Out of memory\n");
	ret = -1;
CLEANUP:
	_te_ast_module_del(pmodule);
	free(pmodule);
	return ret;
}
