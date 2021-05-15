#define TEDLANG_SRC

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include <tedlang/core/eval.h>
#include <tedlang/util/string.h>
#include <tedlang/builtin/bool.h>
#include <tedlang/builtin/int.h>
#include <tedlang/builtin/str.h>
#include <tedlang/builtin/fn.h>

static bool inerror = false;
static bool inbreak = false;
static bool incontinue = false;
static te_obj_st* plastret = NULL;

te_scope_st global_scope = {
	NULL,
	0,
	0,
	0,
	NULL
};

void te_init()
{

}

void* te_seterr(const char* err, ...)
{

}

te_obj_st** te_lval(te_scope_st* pscope, const te_ast_st* past)
{
}

te_obj_st* eval_empty(te_scope_st* pscope, const te_ast_noexpr_st* pempty)
{
	// TODO: create the builtin type: null
}

te_obj_st* eval_var(te_scope_st* pscope, te_ast_var_st* pvar)
{
	te_obj_st** ppobj = te_scope_get(pscope, pvar->name);
	if (ppobj)
		return *ppobj;
	return te_seterr("Reference to undefined variable '%s'", pvar->name);
}

te_obj_st* eval_null(te_scope_st* pscope, te_ast_null_st* pnull)
{
	// TODO: create the builtin type: null
}

te_obj_st* eval_bool(te_scope_st* pscope, te_ast_bool_st* pbool)
{
	te_bool_st* npbool = te_bool_new();
	if (te_haserr())
		return NULL;
	npbool->val = pbool->val;
	return npbool;
}

te_obj_st* eval_int(te_scope_st* pscope, te_ast_int_st* pint)
{
	te_int_st* npint = te_int_new();
	if (te_haserr())
		return NULL;
	npint->val = pint->val;
	return npint;
}

te_obj_st* eval_str(te_scope_st* pscope, te_ast_str_st* pstr)
{
	te_str_st* npstr = te_str_new();
	if (te_haserr())
		return NULL;
	size_t sz = te_strlen(pstr->val);
	npstr->val = (char*)malloc(sizeof(char) * sz);
	if (!npstr->val)
	{
		te_decref(npstr);
		return NULL;
	}
	te_memcpy(npstr->val, pstr->val, sz);
	return npstr;
}

te_obj_st* eval_arr(te_scope_st* pscope, const te_ast_arr_st* parr)
{
	// TODO: create the builtin type: array
}

te_obj_st* eval_seq(te_scope_st* pscope, const te_ast_seq_st* pseq)
{
	te_obj_st* ptmp;
	for (size_t i = 0; i < pseq->expr_num; i++)
	{
		ptmp = te_eval(pscope, pseq->ppnds[i]);
		if (ptmp)
			te_decref(ptmp);
		if (te_haserr() || plastret || inbreak || incontinue)
			break;
	}
	return NULL;
}

te_obj_st* eval_for(te_scope_st* pscope, const te_ast_for_st* pfor)
{
	te_obj_st* piter = te_eval(pscope, pfor->iter);
	if (te_haserr())
		return NULL;
	if (!piter)
		return te_seterr("For loop does not contain an iterable value");

	te_obj_st* pctx = te_start(piter);
	if (te_haserr())
		goto CLEAN_UP;
	if (inbreak || incontinue)
	{
		te_seterr("Invalid state for starting a for loop");
		goto CLEAN_UP;
	}

	te_scope_st* pret;
	te_obj_st* pval;
	while (true)
	{
		pval = te_next(piter, pctx);
		if (te_haserr() || !pval)
			break;

		pret = te_scope_set(pscope, pfor->it, pval);
		te_decref(pval);
		if (!pret)
		{
			te_seterr("Unable to add variable '%s' to the scope", pfor->it);
			break;
		}
		te_decref_s(te_eval(pscope, pfor->body));
		if (te_haserr() || plastret)
			break;

		if (inbreak)
		{
			inbreak = false;
			break;
		}
		if (incontinue)
		{
			incontinue = false;
			continue;
		}
	}

CLEAN_UP:
	te_decref_s(pctx);
	te_decref(piter);
	return NULL;
}

te_obj_st* eval_while(te_scope_st* pscope, const te_ast_while_st* pwhile)
{
	te_obj_st* pcond;
	while (true)
	{
		pcond = te_eval(pscope, pwhile->cond);
		if (te_haserr())
			return NULL;
		if (!pcond)
			return te_seterr("Invalid while loop condition");
		if (!te_bool(pcond))
		{
			te_decref(pcond);
			break;
		}
		te_decref(pcond);
		te_decref_s(te_eval(pscope, pwhile->body));
		if (te_haserr() || plastret)
			return NULL;

		if (inbreak)
		{
			inbreak = false;
			break;
		}
		if (incontinue)
		{
			incontinue = false;
			continue;
		}
	}
}

te_obj_st* eval_break(te_scope_st* pscope, const te_ast_break_st* pbreak)
{
	inbreak = true;
	return NULL;
}

te_obj_st* eval_continue(te_scope_st* pscope, const te_ast_continue_st* pcontinue)
{
	incontinue = true;
	return NULL;
}

te_obj_st* eval_branch(te_scope_st* pscope, const te_ast_branch_st* pbranch)
{
	te_obj_st* pcond = te_eval(pscope, pbranch->cond);
	if (te_haserr())
		return NULL;
	if (!pcond)
		return te_seterr("Invalid if statement condition");

	// errors can be ignored here, they'll be propagated back
	te_obj_st* ptmp = NULL;
	if (te_bool(pcond))
		ptmp = te_eval(pscope, pbranch->if_body);
	else if (pbranch->else_body)
		ptmp = te_eval(pscope, pbranch->else_body);
	
	te_decref_s(ptmp);
	te_decref(pcond);
	return NULL;
}

te_obj_st* eval_return(te_scope_st* pscope, const te_ast_return_st* preturn)
{
	te_obj_st* pret = NULL;
	if (preturn->ret)
	{
		pret = te_eval(pscope, preturn->ret);
		if (te_haserr())
			return NULL;
		if (!pret)
			return te_seterr("Invalid return value");
	}
	else
	{
		// TODO: create the builtin type: null
	}

	plastret = pret;
	return NULL;
}

te_obj_st* eval_call(te_scope_st* pscope, const te_ast_call_st* pcall)
{
}

te_obj_st* eval_fn(te_scope_st* pscope, te_ast_fn_st* pfn)
{
	te_fn_st* npfn = te_fn_new();
	if (te_haserr())
		return NULL;
	te_scope_st* pfnscope = te_scope_set(pscope, pfn->name, npfn);
	if (!pfnscope)
	{
		te_decref(npfn);
		return te_seterr("Out of memory");
	}
	npfn->pscope = pfnscope;

	// moving the members of the ast node into the new object, preventing an unnessisary expensive copy
	npfn->pbody = pfn->pbody;
	npfn->argc = pfn->argc;
	npfn->ppargv = pfn->ppargv;

	pfn->pbody = NULL;
	pfn->ppargv = NULL;

	return NULL;
}

te_obj_st* eval_imp(te_scope_st* pscope, const te_ast_imp_st* pimp)
{
}

te_obj_st* eval_module(te_scope_st* pscope, const te_ast_module_st* pmodule)
{
}

#define fn_eval_iop(TY)                                                      \
te_obj_st* eval_##TY(te_scope_st* pscope, const te_ast_##TY##_st* piop)      \
{																			 \
	te_obj_st* prval = te_eval(pscope, piop->rval);						     \
	if (te_haserr()) return NULL;											 \
	if (!prval)      return te_seterr("Invalid r-value for assignment");	 \
	te_obj_st** pplval = te_lval(pscope, piop->lval);					     \
	if (te_haserr()) return NULL;											 \
	assert(pplval && *pplval);												 \
	te_obj_st* pret = te_assign(pplval, prval);								 \
	if (te_haserr()) {														 \
		te_decref(pret);													 \
		return NULL;}														 \
	te_incref_s(pret);														 \
	te_decref(prval);														 \
	return pret;															 \
}

fn_eval_iop(assign)
fn_eval_iop(iadd)
fn_eval_iop(isub)
fn_eval_iop(imul)
fn_eval_iop(idiv)
fn_eval_iop(imod)
fn_eval_iop(iexp)
#undef fn_eval_iop

#define fn_eval_cmp_op(TY)                                                \
te_obj_st* eval_##TY(te_scope_st* pscope, const te_ast_##TY##_st* pcmp)   \
{                                                                         \
	te_obj_st* plval = te_eval(pscope, pcmp->lval);                       \
	if (te_haserr())                                                      \
		return NULL;                                                      \
	if (!plval)                                                           \
		return te_seterr("Invalid l-value for comparison");               \
	te_obj_st* prval = te_eval(pscope, pcmp->rval);                       \
	if (te_haserr()) {                                                    \
		te_decref(plval);                                                 \
		return NULL;}                                                     \
	if (!prval) {                                                         \
		te_decref(plval);                                                 \
		return te_seterr("Invalid r-value for comparison");}              \
	te_obj_st* presult = te_##TY(plval, prval);                           \
	if (te_haserr()) {                                                    \
		te_decref(prval);                                                 \
		te_decref(plval);                                                 \
		te_decref_s(presult);                                             \
		return NULL;}                                                     \
	if (!presult) {                                                       \
		te_decref(prval);                                                 \
		te_decref(plval);                                                 \
		te_decref_s(presult);                                             \
		return te_seterr("Invalid comparison result");}                   \
	te_incref(presult);                                                   \
	te_decref(plval);                                                     \
	te_decref(prval);                                                     \
	return presult;                                                       \
}

fn_eval_cmp_op(eq)
fn_eval_cmp_op(ne)
fn_eval_cmp_op(lt)
fn_eval_cmp_op(gt)
fn_eval_cmp_op(le)
fn_eval_cmp_op(ge)
#undef fn_eval_cmp_op

te_obj_st* eval_and(te_scope_st* pscope, const te_ast_and_st* pand)
{
}

te_obj_st* eval_or(te_scope_st* pscope, const te_ast_or_st* por)
{
}

te_obj_st* eval_add(te_scope_st* pscope, const te_ast_add_st* padd)
{
}

te_obj_st* eval_sub(te_scope_st* pscope, const te_ast_sub_st* psub)
{
}

te_obj_st* eval_mul(te_scope_st* pscope, const te_ast_mul_st* pmul)
{
}

te_obj_st* eval_div(te_scope_st* pscope, const te_ast_div_st* pdiv)
{
}

te_obj_st* eval_mod(te_scope_st* pscope, const te_ast_mod_st * pexp)
{
}

te_obj_st* eval_exp(te_scope_st* pscope, const te_ast_exp_st* pexp)
{
}

te_obj_st* eval_idx(te_scope_st* pscope, const te_ast_idx_st* pidx)
{
}

te_obj_st* te_eval(te_scope_st* pscope, te_ast_st* past)
{
	if (te_haserr())
		return NULL;

	// polymorphism in C!
	switch (past->ast_ty)
	{
	case AST_EMPTY:     return eval_empty    (pscope, past);
	case AST_VAR:       return eval_var      (pscope, past);
	case AST_NULL:      return eval_null     (pscope, past);
	case AST_BOOL:      return eval_bool     (pscope, past);
	case AST_INT:       return eval_int      (pscope, past);
	case AST_STR:       return eval_str      (pscope, past);
	case AST_ARR:       return eval_arr      (pscope, past);
	case AST_SEQ:       return eval_seq      (pscope, past);
	case AST_FOR:       return eval_for      (pscope, past);
	case AST_WHILE:     return eval_while    (pscope, past);
	case AST_BREAK:     return eval_break    (pscope, past);
	case AST_CONTINUE:  return eval_continue (pscope, past);
	case AST_BRANCH:    return eval_branch   (pscope, past);
	case AST_RETURN:    return eval_return   (pscope, past);
	case AST_CALL:      return eval_call     (pscope, past);
	case AST_FN:        return eval_fn       (pscope, past);
	case AST_IMP:       return eval_imp      (pscope, past);
	case AST_MODULE:    return eval_module   (pscope, past);
	case AST_ASSIGN:    return eval_assign   (pscope, past);
	case AST_IADD:      return eval_iadd     (pscope, past);
	case AST_ISUB:      return eval_isub     (pscope, past);
	case AST_IMUL:      return eval_imul     (pscope, past);
	case AST_IDIV:      return eval_idiv     (pscope, past);
	case AST_IMOD:      return eval_imod     (pscope, past);
	case AST_IEXP:      return eval_iexp     (pscope, past);
	case AST_EQ:        return eval_eq       (pscope, past);
	case AST_NE:        return eval_ne       (pscope, past);
	case AST_LT:        return eval_lt       (pscope, past);
	case AST_GT:        return eval_gt       (pscope, past);
	case AST_LE:        return eval_le       (pscope, past);
	case AST_GE:        return eval_ge       (pscope, past);
	case AST_AND:       return eval_and      (pscope, past);
	case AST_OR:        return eval_or       (pscope, past);
	case AST_ADD:       return eval_add      (pscope, past);
	case AST_SUB:       return eval_sub      (pscope, past);
	case AST_MUL:       return eval_mul      (pscope, past);
	case AST_DIV:       return eval_div      (pscope, past);
	case AST_MOD:       return eval_mod      (pscope, past);
	case AST_EXP:       return eval_exp      (pscope, past);
	case AST_IDX:       return eval_idx      (pscope, past);
	default:
		assert(false);
		return NULL;  // tmp
	}
}
