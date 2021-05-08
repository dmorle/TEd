#define SRC

#include <stdio.h>
#include <shared.h>

void fn()
{
	printf("printed from libtest.dll\n");
}

void get_fp(test_fn_st* p)
{
	p->m_fn = fn;
}
