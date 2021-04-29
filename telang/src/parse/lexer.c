#define TE_SRC

#include <telang/parse/lexer.h>

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

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
	case '+':
		if (bufsz > 1 && ptoken[1] == '=')
		{
			ntoken.t_id = TK_OP_IADD;
			ret = 2;
		}
		else
			ntoken.t_id = TK_OP_ADD;
		break;
	case '-':
		if (bufsz > 1 && ptoken[1] == '=')
		{
			ntoken.t_id = TK_OP_ISUB;
			ret = 2;
		}
		else
			ntoken.t_id = TK_OP_SUB;
		break;
	case '*':
		if (bufsz > 1 && ptoken[1] == '=')
		{
			ntoken.t_id = TK_OP_IMUL;
			ret = 2;
		}
		else
			ntoken.t_id = TK_OP_MUL;
		break;
	case '/':
		if (bufsz > 1 && ptoken[1] == '=')
		{
			ntoken.t_id = TK_OP_IDIV;
			ret = 2;
		}
		else
			ntoken.t_id = TK_OP_DIV;
		break;
	case '^':
		if (bufsz > 1 && ptoken[1] == '=')
		{
			ntoken.t_id = TK_OP_IEXP;
			ret = 2;
		}
		else
			ntoken.t_id = TK_OP_EXP;
		break;
	case '!':
		if (bufsz > 1 && ptoken[1] == '=')
		{
			ntoken.t_id = TK_OP_NE;
			ret = 2;
		}
		else
			ntoken.t_id = TK_OP_NOT;
		break;
	case '<':
		if (bufsz > 1 && ptoken[1] == '=')
		{
			ntoken.t_id = TK_OP_LE;
			ret = 2;
		}
		else
			ntoken.t_id = TK_OP_LT;
		break;
	case '>':
		if (bufsz > 1 && ptoken[1] == '=')
		{
			ntoken.t_id = TK_OP_GE;
			ret = 2;
		}
		else
			ntoken.t_id = TK_OP_GT;
		break;
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
			memcpy(ntoken._data, (void*)(ptoken + 1), (size_t)ret - 2);
			((char*)ntoken._data)[ret - 2] = '\0';
			_te_tarr_append(ptarr, &ntoken);
			return ret;
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

			switch (ret)
			{
			case 2:
				if (!memcmp(ptoken - 2, "if", 2))
				{
					ntoken.t_id = TK_IF;
					goto RD_TK_END;
				}
				if (!memcmp(ptoken - 2, "fn", 2))
				{
					ntoken.t_id = TK_FN;
					goto RD_TK_END;
				}
				break;
			case 3:
				if (!memcmp(ptoken - 3, "for", 3))
				{
					ntoken.t_id = TK_FOR;
					goto RD_TK_END;
				}
				break;
			case 4:
				if (!memcmp(ptoken - 4, "null", 4))
				{
					ntoken.t_id = TK_NULL;
					goto RD_TK_END;
				}
				if (!memcmp(ptoken - 4, "true", 4))
				{
					ntoken.t_id = TK_TRUE;
					goto RD_TK_END;
				}
				if (!memcmp(ptoken - 4, "else", 4))
				{
					ntoken.t_id = TK_ELSE;
					goto RD_TK_END;
				}
				break;
			case 5:
				if (!memcmp(ptoken - 5, "false", 5))
				{
					ntoken.t_id = TK_FALSE;
					goto RD_TK_END;
				}
				if (!memcmp(ptoken - 5, "while", 5))
				{
					ntoken.t_id = TK_WHILE;
					goto RD_TK_END;
				}
				break;
			case 6:
				if (!memcmp(ptoken - 6, "return", 6))
				{
					ntoken.t_id = TK_RETURN;
					goto RD_TK_END;
				}
				break;
			}

			ntoken.t_id = TK_IDN;
			ntoken._data = malloc(sizeof(char) * ((size_t)ret + 1));
			if (!ntoken._data)
				return -1;
			memcpy(ntoken._data, (void*)(ptoken - ret), (size_t)ret);
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
