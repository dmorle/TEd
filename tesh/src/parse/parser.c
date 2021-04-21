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

// Determines the position of the final token contained within a set of brackets for a given token array
int te_find_brac_end(const te_tarr_st* ptarr, size_t start, te_token_et to_id, te_token_et tc_id)
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
int te_find_tk(const te_tarr_st* ptarr, size_t start, te_token_et tk)
{
	for (size_t i = start; i < ptarr->length; i++)
		if (ptarr->_data[i].t_id == tk)
			return i;

	return -2;
}

// Finds the position of the end of a 'block'
int te_find_block_end(const te_tarr_st* ptarr, size_t start)
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
				return i;
			break;
		}
	}

	return -2;
}

// Looks through the token array until a top level ',' or closing bracket is found
int te_find_elem_end(const te_tarr_st* ptarr, size_t idx, te_token_et tk_open, te_token_et tk_close)
{
	// token values could be passed at compile time, but idk if its worth it
	int brac = 0;
	int brac = 0;
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
	return -2;
}

// Parses comma seperated array elements
int te_parse_arr(const te_tarr_st* ptarr, te_ast_arr_st* parr)
{
	te_ast_st* pelem;
	te_tarr_st tk_slice;
	size_t idx = 0;
	int ret;

	while (idx < ptarr->length)
	{
		ret = te_find_elem_end(ptarr, idx, TK_O_SQUARE, TK_C_SQUARE);
		if (ret < 0)
		{
			fprintf(stderr, "Invalid array element on line %zu", ptarr->_data[idx].linenum);
			return ret;
		}
		_te_tarr_slice(ptarr, idx, ret, &tk_slice);
		ret = te_parse_expr(&tk_slice, &pelem);
		if (ret < 0)
			return ret;
		ret = _te_ast_arr_append(parr, pelem);
		if (ret < 0)
		{
			_te_ast_del(pelem);
			return ret;
		}
		if (ptarr->_data[ret].t_id == TK_C_SQUARE)
			return ret;
		idx += (size_t)ret + 1;
	}

	fprintf(stderr, "Invalid array definition on line %zu", ptarr->_data[0].linenum);
	return -2;
}

int te_parse_branch(const te_tarr_st* ptarr, te_ast_branch_st* pbranch)
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
	size_t end;
	int ret;

	if (ptarr->_data[1].t_id != TK_O_ROUND)
	{
		fprintf(stderr, "Unexpected token after 'if' on line %zu\n", ptarr->_data[1].linenum);
		return -2;
	}

	// parsing the condition expression
	start = 2;
	ret = te_find_brac_end(ptarr, start, TK_O_ROUND, TK_C_ROUND);
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
	ret = te_find_block_end(ptarr, start);
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
	start = (size_t)ret + 1;
	if (start < ptarr->length && ptarr->_data[start].t_id == TK_ELSE)
	{
		// Allow for chaining via else if
		_te_tarr_slice(ptarr, start + 1, ptarr->length, &tk_slice);
		ret = te_parse_block(&tk_slice, &past);
		if (ret < 0)
			return ret;
		pbranch->else_body = past;
	}

	return ret + 1;
}

int te_parse_for(const te_tarr_st* ptarr, te_ast_for_st* pfor)
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

	ret = te_find_brac_end(ptarr, 4, TK_O_ROUND, TK_C_ROUND);
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
	idx += ret + 1;
	pfor->iter = past;
	_te_tarr_slice(ptarr, (size_t)ret + 1, ptarr->length, &tk_slice);
	ret = te_parse_block(&tk_slice, &past);
	if (ret < 0)
		return ret;
	ret += idx + 1;
	pfor->body = past;
	return ret;
}

int te_parse_while(const te_tarr_st* ptarr, te_ast_while_st* pwhile)
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
}

int te_parse_seq(const te_tarr_st* ptarr, te_ast_seq_st* pseq)
{
	te_tarr_st tk_slice;
	te_ast_st* past;
	size_t idx = 0;
	int ret;

	while (idx < ptarr->length)
	{
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
	return idx;
}

int te_parse_expr(const te_tarr_st* ptarr, te_ast_st** ppexpr)
{
	// 
}

int te_parse_block(const te_tarr_st* ptarr, te_ast_st** ppblock)
{
	if (!ptarr->length)
	{
		fprintf(stderr, "Received invalid code block\n");
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
		ret = te_parse_branch(ptarr, pbranch);
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
		ret = te_parse_for(ptarr, pfor);
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
		ret = te_parse_while(ptarr, pwhile);
		if (ret < 0)
		{
			_te_ast_while_del(pwhile);
			free(pwhile);
			return ret;
		}
		*ppblock = pwhile;
		return ret;
	}
	case TK_RETURN:
	{
		te_ast_return_st* preturn = (te_ast_return_st*)malloc(sizeof(te_ast_return_st));
		if (!preturn)
			goto OUT_OF_MEMORY;
		_te_ast_return_new(preturn);
		ret = te_parse_return(ptarr, preturn);
		if (ret < 0)
		{
			_te_ast_return_del(preturn);
			free(preturn);
			return ret;
		}
		*ppblock = preturn;
		return ret;
	}
	case TK_O_BRACE:
	{
		te_ast_seq_st* pseq = (te_ast_seq_st*)malloc(sizeof(te_ast_seq_st));
		if (!pseq)
			goto OUT_OF_MEMORY;
		_te_ast_seq_new(pseq, 5);
		ret = te_parse_seq(ptarr, pseq);
		if (ret < 0)
		{
			_te_ast_seq_del(pseq);
			free(pseq);
			return ret;
		}
		*ppblock = pseq;
		return ret;
	}
	default:
		te_parse_expr(ptarr, ppblock);
		break;
	}

OUT_OF_MEMORY:
	fprintf(stderr, "Out of memory\n");
	return -1;
}

// returns the number of tokens read on success, -1 on error
int te_parse_module(const te_tarr_st* ptarr, te_ast_st** ppast)
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
	case TK_ITER	  :
		break;
	case TK_ENDL	  :
		break;
	case TK_OP_NOT	  :
		break;
	case TK_TRUE      :
		break;
	case TK_FALSE     :
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
		else if (!strcmp((char*)ptoken->_data, "fn"))
		{

		}
		else if (!strcmp((char*)ptoken->_data, "import"))
		{

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
