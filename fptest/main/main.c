#include <stdio.h>
#include <shared.h>

int main()
{
	test_fn_st s;
	get_fp(&s);
	s.m_fn();
}
