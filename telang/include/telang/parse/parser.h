#ifndef TE_PARSER_H
#define TE_PARSER_H

#include <stdbool.h>

#include <telang/telapi.h>

#include <telang/core/obj.h>
#include <telang/parse/lexer.h>

typedef enum
{
	// data nodes
	AST_EMPTY,
	AST_VAR,
	AST_NULL,
	AST_BOOL,
	AST_INT,
	AST_STR,
	AST_ARR,

	// internal nodes
	AST_MODULE,
	AST_IMP,
	AST_FN,
	AST_CALL,
	AST_RETURN,
	AST_SEQ,
	AST_IDX,
	AST_FOR,
	AST_WHILE,
	AST_BRANCH,

	// binary operators
	AST_ASSIGN,
	AST_IADD,
	AST_ISUB,
	AST_IMUL,
	AST_IDIV,
	AST_IMOD,
	AST_IEXP,
	AST_EQ,
	AST_NE,
	AST_LT,
	AST_GT,
	AST_LE,
	AST_GE,
	AST_AND,
	AST_OR,
	AST_ADD,
	AST_SUB,
	AST_MUL,
	AST_DIV,
	AST_MOD,
	AST_EXP,
	AST_IDX, // ex. a[0]

	// unary operators
	AST_NOT
}
te_ast_et;

// base class for all ast nodes
// returns either te_obj_st* or NULL on eval, depending on type
typedef struct
{
	te_ast_et ast_ty;
}
te_ast_st;

// deallocates a generic ast node
TE_API void _te_ast_del(te_ast_st* pself);

// data nodes

// empty expression
// returns NULL on eval
typedef struct
{
	te_ast_st super;
}
te_ast_noexpr_st;

TE_API void _te_ast_noexpr_new(te_ast_noexpr_st* pself);
TE_API void _te_ast_noexpr_del(te_ast_noexpr_st* pself);

// refers to a te_obj_st* in a hash table containing global variables
// returns the retrieved te_obj_st* on eval
typedef struct
{
	te_ast_st super;
	char* name;  // dynamically allocated NULL terminated string
}
te_ast_var_st;

TE_API void _te_ast_var_new(te_ast_var_st* pself);
TE_API void _te_ast_var_del(te_ast_var_st* pself);

// represents a null value in tedlang
// returns te_null_st* on eval
typedef struct
{
	te_ast_st super;
}
te_ast_null_st;

TE_API void _te_ast_null_new(te_ast_null_st* pself);
TE_API void _te_ast_null_del(te_ast_null_st* pself);

// contains a boolean value
// returns te_bool_st* on eval
typedef struct
{
	te_ast_st super;
	bool val;
}
te_ast_bool_st;

TE_API void _te_ast_bool_new(te_ast_bool_st* pself);
TE_API void _te_ast_bool_del(te_ast_bool_st* pself);

// contains an integer
// returns te_int_st* on eval
typedef struct
{
	te_ast_st super;
	int64_t val;
}
te_ast_int_st;

TE_API void _te_ast_int_new(te_ast_int_st* pself);
TE_API void _te_ast_int_del(te_ast_int_st* pself);

// contains a NULL terminated string
// returns te_str_st* on eval
typedef struct
{
	te_ast_st super;
	char* val;
}
te_ast_str_st;

TE_API void _te_ast_str_new(te_ast_str_st* pself);
TE_API void _te_ast_str_del(te_ast_str_st* pself);

// contains an array of te_ast_st* which contain the elements of the array
// returns te_arr_st* on eval
typedef struct
{
	te_ast_st super;
	size_t length;
	size_t _mem_sz;
	te_ast_st** pelems;
}
te_ast_arr_st;

TE_API int _te_ast_arr_new(te_ast_arr_st* pself, size_t sz);
TE_API void _te_ast_arr_del(te_ast_arr_st* pself);

TE_API int _te_ast_arr_append(te_ast_arr_st* pself, te_ast_st* pelem);

// special cases for expressions

// returns NULL on eval
typedef struct
{
	te_ast_st super;
}
te_ast_break_st;

TE_API void _te_ast_break_new(te_ast_break_st* pself);
TE_API void _te_ast_break_del(te_ast_break_st* pself);

// returns NULL on eval
typedef struct
{
	te_ast_st super;
}
te_ast_continue_st;

TE_API void _te_ast_continue_new(te_ast_continue_st* pself);
TE_API void _te_ast_continue_del(te_ast_continue_st* pself);

// internal nodes

// returns NULL on eval
typedef struct
{
	te_ast_st super;
	char* imp_name;  // NULL terminated string
}
te_ast_imp_st;

// function definition
// stores local variables, runs pbody, restores local variables
typedef struct
{
	te_ast_st super;
	te_ast_st* pbody;
	size_t argc;
	size_t _mem_sz;
	char** ppargv;  // NULL terminated strings
	char* name;     // NULL terminated string
}
te_ast_fn_st;

// contains an array of te_ast_st*
// returns NULL on eval
typedef struct
{
	te_ast_st super;
	size_t expr_num;  // number of elements in ppnds
	size_t _mem_sz;
	te_ast_st** ppnds;
}
te_ast_seq_st;

// returns -1 on failure
TE_API int _te_ast_seq_new(te_ast_seq_st* pself, size_t sz);
TE_API void _te_ast_seq_del(te_ast_seq_st* pself);

// returns -1 on failure
TE_API int _te_ast_seq_append(te_ast_seq_st* pself, te_ast_st* pexpr);

// represents the contents of any tesh file
// returns NULL on eval
typedef struct
{
	te_ast_st super;

	size_t imp_num;
	size_t imp_mem_sz;
	te_ast_imp_st* pimps;

	size_t fn_num;
	size_t fn_mem_sz;
	te_ast_fn_st* pfns;

	size_t expr_num;
	size_t expr_mem_sz;
	te_ast_st** ppexprs;
}
te_ast_module_st;

// iterates through an iterable type iter (string, array), passing each element to idx
// returns NULL on eval
typedef struct
{
	te_ast_st super;
	te_ast_st* body;
	te_ast_st* iter;  // Must return an iterable on eval
	char* it;         // name of the varible used during iteration
}
te_ast_for_st;

TE_API void _te_ast_for_new(te_ast_for_st* pself);
TE_API void _te_ast_for_del(te_ast_for_st* pself);

// evals body until cond returns false
// returns NULL on eval
typedef struct
{
	te_ast_st super;
	te_ast_st* body;
	te_ast_st* cond;  // Must return te_bool_st* on eval
}
te_ast_while_st;

TE_API void _te_ast_while_new(te_ast_while_st* pself);
TE_API void _te_ast_while_del(te_ast_while_st* pself);

// evals cond, true => eval(if_body), false => eval(else_body)
// returns NULL on eval
typedef struct
{
	te_ast_st super;
	te_ast_st* if_body;
	te_ast_st* else_body;
	te_ast_st* cond;  // Must return te_bool_st* on eval
}
te_ast_branch_st;

TE_API void _te_ast_branch_new(te_ast_branch_st* pself);
TE_API void _te_ast_branch_del(te_ast_branch_st* pself);

// calls a tesh function
// returns either te_obj_st* or NULL on eval depending on the function
typedef struct
{
	te_ast_st super;
	size_t argc;
	size_t _mem_sz;
	te_ast_st** ppargv;
	te_ast_st* pfn;   // must be a function reference, otherwise eval will fail
}
te_ast_call_st;

TE_API int _te_ast_call_new(te_ast_call_st* pself, size_t sz);
TE_API void _te_ast_call_del(te_ast_call_st* pself);

TE_API int _te_ast_call_append(te_ast_call_st* pself, te_ast_st* parg);

// exit point of a function
// returns te_obj_st* or NULL on eval depending on ret
typedef struct
{
	te_ast_st super;
	te_ast_st* ret;  // Optional parameter; if set may return either NULL or te_obj_st*
}
te_ast_return_st;

TE_API void _te_ast_return_new(te_ast_return_st* pself);
TE_API void _te_ast_return_del(te_ast_return_st* pself);

// Binary operators

// generic struct for assignment based operators
// returns `name` on eval
typedef struct
{
	te_ast_st super;
	te_ast_st* rval;
	char* name;
}
te_ast_p0_st;

TE_API void _te_ast_p0_new(te_ast_p0_st* pself, te_ast_et ast_ty);
TE_API void _te_ast_p0_del(te_ast_p0_st* pself);

typedef te_ast_p0_st
        te_ast_assign_st,
        te_ast_iadd_st,
        te_ast_isub_st,
        te_ast_imul_st,
        te_ast_idiv_st,
        te_ast_imod_st,
        te_ast_iexp_st;
#define _te_ast_assign_new( pself ) _te_ast_p0_new( pself, AST_ASSIGN )
#define   _te_ast_iadd_new( pself ) _te_ast_p0_new( pself, AST_IADD   )
#define   _te_ast_isub_new( pself ) _te_ast_p0_new( pself, AST_ISUB   )
#define   _te_ast_imul_new( pself ) _te_ast_p0_new( pself, AST_IMUL   )
#define   _te_ast_idiv_new( pself ) _te_ast_p0_new( pself, AST_IDIV   )
#define   _te_ast_imod_new( pself ) _te_ast_p0_new( pself, AST_IMOD   )
#define   _te_ast_iexp_new( pself ) _te_ast_p0_new( pself, AST_IEXP   )

// generic struct for non-assignment based binary operators
// returns te_obj_st* on eval
typedef struct
{
	te_ast_st super;
	te_ast_st* lval;
	te_ast_st* rval;
}
te_ast_bin_st;

TE_API void _te_ast_bin_new(te_ast_bin_st* pself, te_ast_et ast_ty);
TE_API void _te_ast_bin_del(te_ast_bin_st* pself);

typedef te_ast_bin_st
        te_ast_eq_st,
        te_ast_ne_st,
        te_ast_lt_st,
        te_ast_gt_st,
        te_ast_le_st,
        te_ast_ge_st,
#define  _te_ast_eq_new( pself ) _te_ast_bin_new( pself, AST_EQ  )
#define  _te_ast_ne_new( pself ) _te_ast_bin_new( pself, AST_NE  )
#define  _te_ast_lt_new( pself ) _te_ast_bin_new( pself, AST_LT  )
#define  _te_ast_gt_new( pself ) _te_ast_bin_new( pself, AST_GT  )
#define  _te_ast_le_new( pself ) _te_ast_bin_new( pself, AST_LE  )
#define  _te_ast_ge_new( pself ) _te_ast_bin_new( pself, AST_GE  )

        te_ast_and_st,
        te_ast_or_st,
#define _te_ast_and_new( pself ) _te_ast_bin_new( pself, AST_AND )
#define  _te_ast_or_new( pself ) _te_ast_bin_new( pself, AST_OR  )

        te_ast_add_st,
        te_ast_sub_st,
        te_ast_mul_st,
        te_ast_div_st,
        te_ast_mod_st,
        te_ast_exp_st,
        te_ast_idx_st;
#define _te_ast_add_new( pself ) _te_ast_bin_new( pself, AST_ADD )
#define _te_ast_sub_new( pself ) _te_ast_bin_new( pself, AST_SUB )
#define _te_ast_mul_new( pself ) _te_ast_bin_new( pself, AST_MUL )
#define _te_ast_div_new( pself ) _te_ast_bin_new( pself, AST_DIV )
#define _te_ast_mod_new( pself ) _te_ast_bin_new( pself, AST_MOD )
#define _te_ast_exp_new( pself ) _te_ast_bin_new( pself, AST_EXP )
#define _te_ast_idx_new( pself ) _te_ast_bin_new( pself, AST_IDX )

TE_API int te_parse_expr   (const te_tarr_st* ptarr, te_ast_st** ppast);
TE_API int te_parse_block  (const te_tarr_st* ptarr, te_ast_st** ppast);
TE_API int te_parse_module (const te_tarr_st* ptarr, te_ast_st** ppast);

#endif
