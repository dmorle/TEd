#ifndef TE_PARSER_H
#define TE_PARSER_H

#include <tesh/core/obj.h>
#include <tesh/parse/lexer.h>

typedef enum
{
	// leaf nodes
	AST_VAR,
	AST_CONST,

	// internal nodes
	AST_IMP,
	AST_FN,
	AST_FILE,
	AST_SEQ,
	AST_FOR,
	AST_WHILE,
	AST_BRANCH,
	AST_CALL,
	AST_RETURN,
	AST_MODULE,

	// operators
	AST_BIN,
	AST_UNI
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
void _te_ast_del(te_ast_st* pself);

// leaf nodes

// refers to a te_obj_st* in a hash table containing global variables
// returns the retrieved te_obj_st* on eval
typedef struct
{
	te_ast_st super;
	char* name;  // NULL terminated string
}
te_ast_var_st;

// contains a dynamically memory allocated te_obj_st*
// returns te_obj_st* on eval
typedef struct
{
	te_ast_st super;
	te_obj_st* pobj;
}
te_ast_const_st;

// internal nodes

// returns NULL on eval
typedef struct
{
	te_ast_st super;
	char* imp_name;  // NULL terminated string
}
te_ast_imp_st;

// calls a tesh function
// returns either te_obj_st* or NULL on eval depending on the function
typedef struct
{
	te_ast_st super;
	size_t argc;
	size_t _mem_sz;
	te_ast_st** ppargv;
	char* name;  // NULL terminated string
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
te_ast_call_st;

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

// types of binary operations: te_ast_bin_st.bin_ty
typedef enum
{
	// effects global state
	AST_BIN_ASSIGN,
	AST_BIN_IADD,
	AST_BIN_ISUB,
	AST_BIN_IMUL,
	AST_BIN_IDIV,
	AST_BIN_IEXP,

	// const global state
	AST_BIN_ADD,
	AST_BIN_SUB,
	AST_BIN_MUL,
	AST_BIN_DIV,
	AST_BIN_EXP,
	AST_BIN_EQ,
	AST_BIN_NE,
	AST_BIN_LT,
	AST_BIN_GT,
	AST_BIN_LE,
	AST_BIN_GE,
	AST_BIN_AND,
	AST_BIN_OR,
	AST_BIN_IDX  // ex. a[0]
}
te_ast_bin_et;

// class for all ast nodes representing binary operations
// returns te_obj_st* on eval
typedef struct
{
	te_ast_st super;
	te_ast_bin_et bin_ty;
	te_ast_st* lval;  // Must return te_obj_st* on eval
	te_ast_st* rval;  // Must return te_obj_st* on eval
}
te_ast_bin_st;

// types of unary operations: te_ast_uni_st.bin_ty
typedef enum
{
	// I think I only have logical not for this...
	AST_UNI_NOT
}
te_ast_uni_et;

// class for all ast nodes representing unary operations
// returns te_obj_st* on eval
typedef struct
{
	te_ast_st super;
	te_ast_uni_et uni_ty;
	te_ast_st* val;  // Must return te_obj_st* on eval
}
te_ast_uni_st;

TE_API int te_parse_expr   (const te_tarr_st* ptarr, te_ast_st** ppast);
TE_API int te_parse_block  (const te_tarr_st* ptarr, te_ast_st** ppast);
TE_API int te_parse_module (const te_tarr_st* ptarr, te_ast_st** ppast);

#endif
