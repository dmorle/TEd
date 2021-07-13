#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tedlang/tedl.h>

int main(int argc, char** argv)
{
	te_tarr_st tarr;
	te_ast_st* past;
	int ret;

	char buf[128];
	if (argc == 2)
		strcpy(buf, argv[1]);
	else
	{
		char* home = getenv("HOMEPATH");
		sprintf(buf, "%s/source/repos/ted/tedlang/samples/hello_world.ted", home);
	}

	FILE* pf = fopen(buf, "rb");
	if (!pf)
	{
		fprintf(stderr, "Unable to open file %s\n", buf);
		exit(1);
	}
	
	ret = _te_tarr_new(&tarr, 128);
	if (ret < 0)
	{
		fprintf(stderr, "Out of memory\n");
		exit(1);
	}

	ret = te_lex_f(pf, &tarr);
	fclose(pf);
	if (ret < 0)
	{
		fprintf(stderr, "Unable to lex %s\n", buf);
		exit(1);
	}

	printf("Finished Lexing...\nTokens:\n\n");
	_te_tarr_print(&tarr);
	printf("\n\n");

	ret = te_parse_module(&tarr, &past);
	_te_tarr_del(&tarr);
	if (ret < 0)
	{
		fprintf(stderr, "Unable to parse %s\n", buf);
		exit(1);
	}
	printf("Finished Parsing\n\n");
	
	te_scope_st* pscope = te_init();
	if (te_haserr())
		exit(1);
	
	printf("\nRunning the script\n");
	te_eval(pscope, past);
	printf("\nFinished running the script\n");
}
