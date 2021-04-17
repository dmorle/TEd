#define TE_SRC

#include <tesh/parse/parser.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

void _te_ast_del(te_ast_st* pself)
{
	// TODO: Implementation
}

int _te_ast_seq_new(te_ast_seq_st* pself, size_t sz)
{
	pself->super.ast_ty = AST_SEQ;
	pself->expr_num = 0;
	pself->ppnds = (te_ast_st**)malloc(sizeof(te_ast_st*) * sz);
	if (!pself->ppnds)
	{
#ifdef _DEBUG
		pself->_mem_sz = 0;  // This will be caught in case of accidental deallocation
#endif
		return -1;
	}
	pself->_mem_sz = sz;
}

void _te_ast_seq_del(te_ast_seq_st* pself)
{
	assert(pself->_mem_sz);

	for (te_ast_st** ppnd = pself->ppnds; ppnd < pself->ppnds + pself->expr_num; ppnd++)
	{
		_te_ast_del(*ppnd);  // recursively release the internal node data
		free(*ppnd);         // release the node itself
	}
	free(pself->ppnds);      // release the node array
}

int _te_ast_seq_append(te_ast_seq_st* pself, te_ast_st* pexpr)
{
	assert(pself->_mem_sz);

	// resize pself->ppnds if needed
	if (pself->expr_num == pself->_mem_sz)
	{
		// doubling strategy
		pself->_mem_sz *= 2;
		void* n_data = realloc(pself->ppnds, sizeof(te_ast_st*) * pself->_mem_sz);
		if (!n_data)
			return -1;
		pself->ppnds = (te_ast_st**)n_data;
	}

	pself->ppnds[pself->expr_num++] = pexpr;
	return 0;
}

void _te_ast_return_new(te_ast_return_st* pself, te_ast_st* pretval)
{
	pself->super.ast_ty = AST_RETURN;
	pself->ret = pretval;
}

// returns the number of tokens read on success, -1 on error
int te_parse(const te_tarr_st* ptarr, te_ast_st** ppast)
{
	te_token_st* ptoken = ptarr->_data;
	switch (ptoken->t_id)
	{
	case TK_O_BRACE   :
	{
		// Look ahead for a closing brace
		size_t ntk = 1;
		size_t bcount = 1;
		while (true)
		{
			if ((ptoken + ntk)->t_id == TK_C_BRACE)
				bcount--;
			if ((ptoken + ntk)->t_id == TK_O_BRACE)
				bcount++;
			if (bcount == 0)
				break;

			if (++ntk == ptarr->length)
			{
				fprintf(stderr, "Unexpected EOF before '}' on line %zu\n", ptoken->linenum);
				return -1;
			}
		}

		te_tarr_st slice;
		_te_tarr_slice(ptarr, 1, ntk, &slice);

		te_ast_seq_st* pseq = (te_ast_seq_st*)malloc(sizeof(te_ast_seq_st));
		if (!pseq)
		{
			fprintf(stderr, "Out of memory\n");
			return -1;
		}
		int ret = _te_ast_seq_new(pseq, 5);
		if (ret < 0)
		{
			free(pseq);
			fprintf(stderr, "Out of memory\n");
			return -1;
		}

		size_t offset = 0;
		te_tarr_st subslice;
		while (offset < slice.length)
		{
			te_ast_st* pexpr;
			_te_tarr_slice(&slice, offset, slice.length, &subslice);
			ret = te_parse(&slice, &pexpr);
			if (ret < 0)
			{
				_te_ast_seq_del(pseq);
				free(pseq);
				return ret;
			}
			offset += ret;
			ret = _te_ast_seq_append(pseq, pexpr);
			if (ret < 0)
			{
				_te_ast_seq_del(pseq);
				free(pseq);
				fprintf(stderr, "Out of memory\n");
				return ret;
			}
		}
		*ppast = pseq;
		return ntk + 1;
	}
	case TK_O_ROUND	  :
		break;
	case TK_O_SQUARE  :
		break;
	case TK_INPUT	  :
		break;
	case TK_ITER	  :
		break;
	case TK_ENDL	  :
		break;
	case TK_OP_NOT	  :
		break;
	case TK_BOOL_LIT  :
		break;
	case TK_INT_LIT	  :
		break;
	case TK_STR_LIT	  :
		break;
	case TK_IDN		  :
		if (!strcmp((char*)ptoken->_data, "if"))
		{

		}
		else if (!strcmp((char*)ptoken->_data, "for"))
		{

		}
		else if (!strcmp((char*)ptoken->_data, "while"))
		{

		}
		else if (!strcmp((char*)ptoken->_data, "return"))
		{
			if (!ptarr->length)
			{
				fprintf(stderr, "Unexpected EOF on line %zu\n", ptoken->linenum);
				return -1;
			}

			int ret;
			te_ast_st* pretval;
			if ((ptoken + 1)->t_id == TK_ENDL)
			{
				pretval = NULL;
				ret = 2;
			}
			else
			{
				size_t ntk = 1;
				while ((ptoken + ntk)->t_id != TK_ENDL)
					if (++ntk == ptarr->length)
					{
						fprintf(stderr, "Unexpected EOF before ';' on line %zu\n", ptoken->linenum);
						return -1;
					}

				te_tarr_st slice;
				_te_tarr_slice(ptarr, 1, ntk + 1, &slice);  // Include the ; as a stopping point

				ret = te_parse(&slice, &pretval);
				if (ret < 0)
					return ret;
				ret++;
			}

			te_ast_return_st* pret = (te_ast_return_st*)malloc(sizeof(te_ast_return_st));
			if (!pret)
			{
				fprintf(stderr, "Out of memory\n");
				return -1;
			}
			_te_ast_return_new(pret, pretval);
			*ppast = pret;
			return ret;
		}
		else
		{
			// Not a keyword

			size_t idx = 0;

		}
		break;
	default:
		fprintf(stderr, "Unexpected token found on line %zu\n", ptoken->linenum);
		return -1;
	}
}

int te_eval(const te_ast_st* past, te_obj_st** ppobj)
{

}
