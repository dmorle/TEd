#define TE_SRC

#include <tedlang/parse/lexer.h>

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>

bool _te_is_op(te_token_et t)
{
	return TK_OP_ASSIGN <= t && t <= TK_OP_EXP;
}

int _te_op_prec(te_token_et t)
{
	switch (t)
	{
	case TK_OP_ASSIGN:
	case TK_OP_IADD:
	case TK_OP_ISUB:
	case TK_OP_IMUL:
	case TK_OP_IDIV:
	case TK_OP_IEXP:
		return 0;  // backward parsing
	case TK_OP_EQ:
	case TK_OP_NE:
	case TK_OP_LT:
	case TK_OP_GT:
	case TK_OP_LE:
	case TK_OP_GE:
		return 1;
	case TK_OP_AND:
	case TK_OP_OR:
		return 2;
	case TK_OP_ADD:
	case TK_OP_SUB:
		return 3;
	case TK_OP_MUL:
	case TK_OP_DIV:
	case TK_OP_MOD:
		return 4;
	case TK_OP_EXP:
		return 5;
	case TK_OP_NOT:
		return 6;
	default:
		return -1;
	}
}

const int MAX_PREC = 6;

void _te_token_print(te_token_st* pt)
{
	switch (pt->t_id)
	{
	case TK_O_BRACE:
		printf("\n{\n");
		break;
	case TK_C_BRACE:
		printf("}");
		break;
	case TK_O_ROUND:
		printf("(");
		break;
	case TK_C_ROUND:
		printf(")");
		break;
	case TK_O_SQUARE:
		printf("[");
		break;
	case TK_C_SQUARE:
		printf("]");
		break;
	case TK_DOT:
		printf(".");
		break;
	case TK_COMMA:
		printf(", ");
		break;
	case TK_ITER:
		printf(" : ");
		break;
	case TK_ENDL:
		printf(";\n");
		break;
	case TK_OP_ASSIGN:
		printf(" = ");
		break;
	case TK_OP_IADD:
		printf(" += ");
		break;
	case TK_OP_ISUB:
		printf(" -= ");
		break;
	case TK_OP_IMUL:
		printf(" *= ");
		break;
	case TK_OP_IDIV:
		printf(" /= ");
		break;
	case TK_OP_IMOD:
		printf(" %= ");
		break;
	case TK_OP_IEXP:
		printf(" ^= ");
		break;
	case TK_OP_EQ:
		printf(" == ");
		break;
	case TK_OP_NE:
		printf(" != ");
		break;
	case TK_OP_LT:
		printf(" < ");
		break;
	case TK_OP_GT:
		printf(" > ");
		break;
	case TK_OP_LE:
		printf(" <= ");
		break;
	case TK_OP_GE:
		printf(" >= ");
		break;
	case TK_OP_AND:
		printf(" && ");
		break;
	case TK_OP_OR:
		printf(" || ");
		break;
	case TK_OP_NOT:
		printf("!");
		break;
	case TK_OP_ADD:
		printf(" + ");
		break;
	case TK_OP_SUB:
		printf(" - ");
		break;
	case TK_OP_MUL:
		printf(" * ");
		break;
	case TK_OP_DIV:
		printf(" / ");
		break;
	case TK_OP_MOD:
		printf(" % ");
		break;
	case TK_OP_EXP:
		printf(" ^ ");
		break;
	case TK_NULL:
		printf("null");
		break;
	case TK_TRUE:
		printf("true");
		break;
	case TK_FALSE:
		printf("false");
		break;
	case TK_INT_LIT:
		printf("%" PRId64, (int64_t)pt->_data);
		break;
	case TK_STR_LIT:
		printf("\"%s\"", (char*)pt->_data);
		break;
	case TK_IDN:
		printf("%s", (char*)pt->_data);
		break;
	case TK_IF:
		printf("if ");
		break;
	case TK_ELSE:
		printf("else ");
		break;
	case TK_FOR:
		printf("for ");
		break;
	case TK_WHILE:
		printf("while ");
		break;
	case TK_BREAK:
		printf("break ");
		break;
	case TK_CONTINUE:
		printf("continue ");
		break;
	case TK_FN:
		printf("fn ");
		break;
	case TK_RETURN:
		printf("return ");
		break;
	case TK_IMPORT:
		printf("import ");
		break;
	default:
		fprintf(stderr, "\nUnrecognized token\n");
	}
}

void _te_token_del(te_token_st* pself)
{
	switch (pself->t_id)
	{
	case TK_STR_LIT:
	case TK_IDN:
		if (pself->_data)
			free(pself->_data);
		return;
	default:
		return;
	}
}

int _te_tarr_new(te_tarr_st* pself, size_t size)
{
	pself->length = 0;
	pself->_mem_sz = size;
	pself->_data = (te_token_st*)malloc(sizeof(te_token_st) * size);
	return -!pself->_data;
}

void _te_tarr_del(te_tarr_st* pself)
{
	// don't delete a slice
	assert(pself->_mem_sz);

	for (te_token_st* pta = pself->_data; pta < pself->_data + pself->length; pta++)
		_te_token_del(pta);
	free(pself->_data);
}

int _te_tarr_append(te_tarr_st* pself, te_token_st* ptoken)
{
	// don't append to a slice
	assert(pself->_mem_sz);

	// resize pself->_data if needed
	if (pself->length == pself->_mem_sz)
	{
		// doubling strategy
		pself->_mem_sz *= 2;
		void* n_data = realloc(pself->_data, sizeof(te_token_st) * pself->_mem_sz);
		if (!n_data)
			return -1;
		pself->_data = (te_token_st*)n_data;
	}

	te_token_st* ptmempos = pself->_data + pself->length++;
	ptmempos->t_id = ptoken->t_id;
	ptmempos->_data = ptoken->_data;
	return 0;
}

void _te_tarr_slice(te_tarr_st* pself, size_t start, size_t end, te_tarr_st* pout)
{
	// Note: slices of slices are allowed
	assert(end >= start);
	assert(pself->length >= end);

	pout->length = end - start;
	pout->_data = pself->_data + start;
	pout->_mem_sz = 0;  // identifies pout as a slice
}

void _te_tarr_print(te_tarr_st* pself)
{
	for (size_t i = 0; i < pself->length; i++)
		_te_token_print(pself->_data + i);
}

// reads a token from a buffer
// returns the number of characters read, or a negative number if the operation failed
int read_token(char* ptoken, size_t bufsz, te_tarr_st* ptarr)
{
	te_token_st ntoken;
	ntoken.linenum = 0;
	int ret = 1;
	char c0 = *ptoken;
	switch (c0)
	{
	case ' ':
	case '\t':
	case '\r':
	case '\n':
	case '\v':
	case '\f':
		return 1;
	case '{':
		ntoken.t_id = TK_O_BRACE;
		break;
	case '}':
		ntoken.t_id = TK_C_BRACE;
		break;
	case '(':
		ntoken.t_id = TK_O_ROUND;
		break;
	case ')':
		ntoken.t_id = TK_C_ROUND;
		break;
	case '[':
		ntoken.t_id = TK_O_SQUARE;
		break;
	case ']':
		ntoken.t_id = TK_C_SQUARE;
		break;
	case '.':
		ntoken.t_id = TK_DOT;
		break;
	case ',':
		ntoken.t_id = TK_COMMA;
		break;
	case ':':
		ntoken.t_id = TK_ITER;
		break;
	case ';':
		ntoken.t_id = TK_ENDL;
		break;
	case '=':
		if (bufsz > 1 && ptoken[1] == '=')
		{
			ntoken.t_id = TK_OP_EQ;
			ret = 2;
		}
		else
			ntoken.t_id = TK_OP_ASSIGN;
		break;
#define LEX_BIN_OP(eop, op)                \
		if (bufsz > 1 && ptoken[1] == '=') \
		{                                  \
			ntoken.t_id = TK_OP_##eop;     \
			ret = 2;                       \
		}                                  \
		else                               \
			ntoken.t_id = TK_OP_##op;      \
		break
#define LEX_BIN_IOP(op) LEX_BIN_OP(I##op, op)
	case '+':
		LEX_BIN_IOP(ADD);
	case '-':
		LEX_BIN_IOP(SUB);
	case '*':
		LEX_BIN_IOP(MUL);
	case '/':
		LEX_BIN_IOP(DIV);
	case '%':
		LEX_BIN_IOP(MOD);
	case '^':
		LEX_BIN_IOP(EXP);
#undef LEX_BIN_IOP
	case '!':
		LEX_BIN_OP(NE, NOT);
	case '<':
		LEX_BIN_OP(LE, LT);
	case '>':
		LEX_BIN_OP(GE, GT);
#undef LEX_BIN_OP
	case '&':
		if (bufsz > 1 && ptoken[1] != '&')
			return -2;
		ntoken.t_id = TK_OP_AND;
		ret = 2;
		break;
	case '|':
		if (bufsz > 1 && ptoken[1] != '|')
			return -2;
		ntoken.t_id = TK_OP_OR;
		ret = 2;
		break;
	default:
		if (c0 == '"')
		{
			while (ptoken[ret] != '"')
				if (++ret == bufsz)
				{
					fprintf(stderr, "EOF reached before closeing \"\n");
					return -2;
				}

			ntoken.t_id = TK_STR_LIT;
			ntoken._data = malloc(sizeof(char) * ((size_t)ret - 1));
			if (!ntoken._data)
				return -1;
			memcpy(ntoken._data, (void*)(ptoken + 1), (size_t)ret - 1);
			((char*)ntoken._data)[ret - 1] = '\0';
			_te_tarr_append(ptarr, &ntoken);
			return ret + 1;
		}
		if (c0 == '-' || ('0' <= c0 && c0 <= '9'))
		{
			// int literal
			int val = ('0' <= c0 && c0 <= '9') * (c0 - '0');
			while (ret < bufsz)
			{
				if (ptoken[ret] <'0' || '9' < ptoken[ret])
					break;

				val *= 10;
				val += ptoken[ret] - '0';
				ret += 1;
			}
			if (c0 == '-')
				val *= -1;
			ntoken.t_id = TK_INT_LIT;
			ntoken._data = (int64_t)val;
			_te_tarr_append(ptarr, &ntoken);
			return ret;
		}
		if (c0 == '_' || ('a' <= c0 && c0 <= 'z') || ('A' <= c0 && c0 <= 'Z'))
		{
			while (
				ptoken[ret] == '_' ||
				('a' <= ptoken[ret] && ptoken[ret] <= 'z') ||
				('A' <= ptoken[ret] && ptoken[ret] <= 'Z') ||
				('0' <= ptoken[ret] && ptoken[ret] <= '9'))
				if (++ret == bufsz)
					break;

#define CHECK_KEYWORD(kw, tk)                                         \
			if (!memcmp(ptoken, kw, sizeof(kw) - 1))                  \
			{                                                         \
				ntoken.t_id = tk;                                     \
				goto RD_TK_END;                                       \
			}

			switch (ret)
			{
			case 2:
				CHECK_KEYWORD("if", TK_IF);
				CHECK_KEYWORD("fn", TK_FN);
				break;
			case 3:
				CHECK_KEYWORD("for", TK_FOR);
				break;
			case 4:
				CHECK_KEYWORD("null", TK_NULL);
				CHECK_KEYWORD("true", TK_TRUE);
				CHECK_KEYWORD("else", TK_ELSE);
				break;
			case 5:
				CHECK_KEYWORD("false", TK_FALSE);
				CHECK_KEYWORD("while", TK_WHILE);
				CHECK_KEYWORD("break", TK_BREAK);
				break;
			case 6:
				CHECK_KEYWORD("return", TK_RETURN);
				CHECK_KEYWORD("import", TK_IMPORT);
				break;
			case 8:
				CHECK_KEYWORD("continue", TK_CONTINUE);
				break;
			}

#undef CHECK_KEYWORD

			ntoken.t_id = TK_IDN;
			ntoken._data = malloc(sizeof(char) * ((size_t)ret + 1));
			if (!ntoken._data)
				return -1;
			memcpy(ntoken._data, (void*)ptoken, (size_t)ret);
			((char*)ntoken._data)[ret] = '\0';
			_te_tarr_append(ptarr, &ntoken);
			return ret;
		}
		fprintf(stderr, "Unexpected character: %c\n", c0);
		return -2;
	}

RD_TK_END:
	ntoken._data = NULL;
	_te_tarr_append(ptarr, &ntoken);
	return ret;
}

int te_lex_f(FILE* pf, te_tarr_st* ptarr)
{
	// temporary bad implementation
	
	fseek(pf, 0, SEEK_END);
	size_t fsz = ftell(pf);
	rewind(pf);
	char* pbuf = (char*)malloc(sizeof(char) * fsz);
	if (!pbuf)
		return -1;
	size_t result = fread(pbuf, 1, fsz, pf);
	if (result != fsz)
	{
		free(pbuf);
		return -2;
	}
	int ret = te_lex_buf(pbuf, fsz, ptarr);
	free(pbuf);
	return ret;
}

int te_lex_buf(char* pbuf, size_t bufsz, te_tarr_st* ptarr)
{
	if (bufsz == 0)
	{
		fprintf(stderr, "Empty files are not allowed\n");
		return -2;
	}

	size_t offset = 0;
	size_t lcount = 1;
	int ret;
	
	while (offset < bufsz)
	{
		ret = read_token(pbuf + offset, bufsz - offset, ptarr);
		if (ret < 0)
		{
			fprintf(stderr, "Error parsing tish script:\n");
			if (ret == -1)
				fprintf(stderr, "Out of memory\n");
			else
				fprintf(stderr, "Syntax error on line %zu\n", lcount);
			return ret;
		}
		ptarr->_data[ptarr->length - 1].linenum = lcount;
		while (ret)
		{
			lcount += pbuf[offset++] == '\n';
			ret--;
		}
	}
	return ret;
}
