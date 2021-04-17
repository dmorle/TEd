#ifndef TI_LEXER_H
#define TI_LEXER_H

#include <stdio.h>
#include <stdint.h>

#include <tesh/teshapi.h>

typedef enum
{
	// brackets
	TK_O_BRACE,   // {
	TK_C_BRACE,   // }
	TK_O_ROUND,   // (
	TK_C_ROUND,   // )
	TK_O_SQUARE,  // [
	TK_C_SQUARE,  // ]
	TK_INPUT,     // $
	TK_ITER,      // :
	TK_ENDL,      // ;

	// operators
	TK_OP_ASSIGN, // =
	TK_OP_IADD,   // +=
	TK_OP_ISUB,   // -=
	TK_OP_IMUL,   // *=
	TK_OP_IDIV,   // /=
	TK_OP_IEXP,   // ^=
	TK_OP_EQ,     // ==
	TK_OP_NE,     // !=
	TK_OP_LT,     // <
	TK_OP_GT,     // >
	TK_OP_LE,     // <=
	TK_OP_GE,     // >=
	TK_OP_ADD,    // +
	TK_OP_SUB,    // -
	TK_OP_MUL,    // *
	TK_OP_DIV,    // /
	TK_OP_EXP,    // ^
	TK_OP_AND,    // &&
	TK_OP_OR,     // ||
	TK_OP_NOT,    // !

	// variable things
	TK_BOOL_LIT,  // true | false
	TK_INT_LIT,   // 1234
	TK_STR_LIT,   // "hello world"
	TK_IDN        // identifier
}
te_token_et;

typedef struct
{
	te_token_et t_id;
	size_t linenum;
	void* _data;
}
te_token_st;

void _te_token_del(te_token_st* pself);

typedef struct
{
	size_t _mem_sz;
	size_t length;
	te_token_st* _data;
}
te_tarr_st;

int _te_tarr_new(te_tarr_st* pself, size_t size);
// All token data is freed polymorphically using _te_token_del
void _te_tarr_del(te_tarr_st* pself);

// A shallow copy of token_st is performed.  So...
// ptoken->_data must be heap allocated, but the struct can be stack allocated
int _te_tarr_append(te_tarr_st* pself, te_token_st* ptoken);

void _te_tarr_slice(te_tarr_st* pself, size_t start, size_t end, te_tarr_st* pout);

TE_API int te_lex_f(FILE* pf, te_tarr_st* ptarr);
TE_API int te_lex_buf(char* pbuf, size_t bufsz, te_tarr_st* ptarr);

#endif
