#ifndef TI_PARSER_H
#define TI_PARSER_H

#include <tesh/core/obj.h>
#include <tesh/parse/lexer.h>

typedef enum
{
	// leaf nodes
	AST_VAR,
	AST_CONST,

	// internal nodes
	AST_SEQ,
	AST_FOR,
	AST_WHILE,
	AST_BRANCH,
	AST_COMMAND,
	AST_RETURN,

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
int _te_ast_seq_new(te_ast_seq_st* pself, size_t sz);
void _te_ast_seq_del(te_ast_seq_st* pself);

// returns -1 on failure
int _te_ast_seq_append(te_ast_seq_st* pself, te_ast_st* pexpr);

// iterates through an iterable type iter (string, array), passing each element to idx
// returns NULL on eval
typedef struct
{
	te_ast_st super;
	te_ast_st* body;
	te_ast_st* iter;  // Must return either te_str_st* or te_arr_st* on eval
	te_ast_var_st* idx;
}
te_ast_for_st;

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

// calls a tish command or tish script
// returns either te_obj_st* or NULL on eval depending on the command
typedef struct
{
	te_ast_st super;
	size_t argc;
	size_t _mem_sz;
	te_ast_st** ppargv;
	char* name;  // NULL terminated string
}
te_ast_command_st;

// exit point of the tish script
// exits the program on eval
typedef struct
{
	te_ast_st super;
	te_ast_st* ret;  // Optional parameter; if set may return either NULL or te_obj_st*
}
te_ast_return_st;

// cannot fail, and doesn't need deallocation
void _te_ast_return_new(te_ast_return_st* pself, te_ast_st* pretval);

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

TI_API int te_parse(const te_tarr_st* ptarr, te_ast_st** ppast);
TI_API int te_eval(const te_ast_st* past, te_obj_st** ppobj);

#endif
