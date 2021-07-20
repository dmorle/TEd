#define TEDLANG_SRC
#define _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include <windows.h>

#include <tedlang/core/eval.h>
#include <tedlang/core/scope.h>
#include <tedlang/core/import.h>
#include <tedlang/parse/parser.h>
#include <tedlang/util/string.h>

#define MAX_IMPDIRS 1024
#define TEDLIB_FOLDER "slib\\"

char* pimpdirs[MAX_IMPDIRS] = { NULL };
size_t nimpdirs = 0;

static HMODULE* phdlls = NULL;
static size_t phdlls_memsz = 0;
static size_t nhdlls = 0;

void te_init_imports()
{
	// Currently only adding the tedslib directory

	char* impdir = (char*)malloc(sizeof(char) * MAX_PATH);
	if (!impdir)
	{
		te_seterr("Out of memory");
		return;
	}

	DWORD offset = GetModuleFileNameA(NULL, impdir, MAX_PATH);
	if (!offset)
	{
		DWORD err = GetLastError();
		te_seterr("Windows error: %lu", err);
		free(impdir);
		return;
	}

	while (impdir[--offset] != '\\' && impdir[offset] != '/');
	if (++offset + sizeof(TEDLIB_FOLDER) >= MAX_PATH)
	{
		te_seterr("tedslib path buffer overflow");
		return;
	}

	memcpy(impdir + offset, TEDLIB_FOLDER, sizeof(TEDLIB_FOLDER));
	void* ret = te_add_impdir(impdir);

	// giving the memory allocated in impdir to te_add_impdir
	if (!te_add_impdir(impdir))
	{
		free(impdir);
		te_seterr("Unable to initialize standard import directories");
	}

	// HMODULE dll file handling

	phdlls = (HMODULE*)malloc(sizeof(HMODULE*));
	if (!phdlls)
	{
		// I think this is all I have to do
		free(impdir);
		te_seterr("Out of memory");
	}
	phdlls_memsz = 1;
}

void te_free_imports()
{
	if (phdlls)
	{
		for (size_t i = 0; i < nhdlls; i++)
			FreeLibrary(phdlls[i]);
		free(phdlls);
	}
}

HMODULE register_dll(HMODULE hdll)
{
	if (nhdlls == phdlls_memsz)
	{
		size_t nsz = phdlls_memsz * 2;
		HMODULE* ptmp = (HMODULE*)realloc(phdlls, sizeof(HMODULE) * nsz);
		if (!ptmp)
			return te_seterr("Out of memory");
		phdlls = ptmp;
		phdlls_memsz = nsz;
	}

	return phdlls[nhdlls++] = hdll;
}

char* te_add_impdir(char* impdir)
{
	if (nimpdirs == MAX_IMPDIRS)
		return NULL;
	return pimpdirs[nimpdirs++] = impdir;
}

#define EXT_SZ 8
static char imppth[MAX_PATH + EXT_SZ];

bool file_exists(LPCTSTR szPath)
{
	DWORD dw_attrib = GetFileAttributes(szPath);
	return (
		dw_attrib != INVALID_FILE_ATTRIBUTES && (
			dw_attrib & (
				FILE_ATTRIBUTE_ARCHIVE |
				FILE_ATTRIBUTE_HIDDEN |
				FILE_ATTRIBUTE_NORMAL |
				FILE_ATTRIBUTE_READONLY
			)
		) && !(
			dw_attrib & FILE_ATTRIBUTE_DIRECTORY
		)
	);
}

te_module_et imp_exists(const char* pth, const te_ast_imp_st* pimp)
{
	static char tedext[EXT_SZ] = ".ted";
	static char dllext[EXT_SZ] = ".tedext";

	memset(imppth, 0, MAX_PATH);

	// building the import path
	size_t modlvl = 0;
	size_t idx = te_strncpy(imppth, pth, MAX_PATH);
	while (true)
	{
		if (idx >= MAX_PATH - 1)
			return TE_MODULE_NONE;
		imppth[idx++] = '\\';
		idx += te_strncpy(imppth + idx, pimp->imp_pth[modlvl], MAX_PATH - idx);
		if (++modlvl == pimp->length)
			break;
	}

	DWORD fattrib;
	memcpy(imppth + idx, dllext, EXT_SZ);
	fattrib = GetFileAttributes(imppth);
	if (fattrib == INVALID_FILE_ATTRIBUTES)
		goto INVALID_FILE;
	if (fattrib & (FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_READONLY))
		return TE_MODULE_BIN;

	memcpy(imppth + idx, tedext, EXT_SZ);
	fattrib = GetFileAttributes(imppth);
	if (fattrib == INVALID_FILE_ATTRIBUTES)
		goto INVALID_FILE;
	if (fattrib & (FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_NORMAL))
		return TE_MODULE_SCRIPT;

	return TE_MODULE_NONE;

INVALID_FILE:
	// TODO: better error message
	te_seterr("Found invalid module on import");
	return TE_MODULE_INV;
}

te_module_st* load_script(te_module_st* pmodule)
{
	FILE* pf = fopen(imppth, "rb");
	if (!pf)
		return te_seterr("Unable to open file: %s", imppth);

	te_tarr_st tarr;
	int ret = _te_tarr_new(&tarr, 128);
	if (ret < 0)
	{
		fclose(pf);
		return te_seterr("Out of memory");
	}
	
	ret = te_lex_f(pf, &tarr);
	fclose(pf);
	if (ret < 0)
	{
		_te_tarr_del(&tarr);
		return NULL;
	}
	
	te_ast_st* past;
	ret = te_parse_module(&tarr, &past);
	_te_tarr_del(&tarr);
	if (ret < 0)
		return NULL;
	
	pmodule->past = past;
	return pmodule;
}

te_module_st* load_bin(te_module_st* pmodule)
{
	HMODULE hdll = LoadLibrary(imppth);
	if (!hdll)
		return te_seterr("Unable to load extension: %s", imppth);

	te_loadmod_t ploadmod = (te_loadmod_t)GetProcAddress(hdll, "te_mod_init");
	if (!ploadmod)
	{
		FreeLibrary(hdll);
		return te_seterr("Missing entry point in extension: %s", imppth);
	}

	pmodule->loadmod = ploadmod;
	if (!register_dll(hdll))
	{
		FreeLibrary(hdll);
		return NULL;
	}

	return pmodule;
}

te_module_st* module_load_script(te_module_st* pmodule, const char* pth)
{
	if (!file_exists(pth))
		return te_seterr("Unable to find scipt file at %s", pth);

	size_t pthlen = te_strlen(pth);
	memcpy(imppth, pth, pthlen + 1);
	pmodule->ty = TE_MODULE_SCRIPT;
	return load_script(pmodule);
}

te_module_st* module_load_bin(te_module_st* pmodule, const char* pth)
{
	if (!file_exists(pth))
		return te_seterr("Unable to find extension file at %s", pth);

	size_t pthlen = te_strlen(pth);
	memcpy(imppth, pth, pthlen + 1);
	pmodule->ty = TE_MODULE_BIN;
	return load_bin(pmodule);
}

te_module_st* module_load_pth(te_module_st* pmodule, const char* pth)
{
	if (!file_exists(pth))
		return te_seterr("Unable to find module file at %s", pth);

	size_t pthlen = te_strlen(pth);
	const char* pthext = pth - 3 + pthlen;
	switch (pthext[0])
	{
	case 't':
		if (strcmp(pthext, "ted"))
			break;
		memcpy(imppth, pth, pthlen + 1);
		pmodule->ty = TE_MODULE_SCRIPT;
		return load_script(pmodule);
	case 'd':
		if (strcmp(pthext, "tedext"))
			break;
		memcpy(imppth, pth, pthlen + 1);
		pmodule->ty = TE_MODULE_BIN;
		return load_bin(pmodule);
	}

	return te_seterr("Invalid file extension for tedlang module: %s", pthext);
}

te_module_st* module_load(te_module_st* pmodule, const te_ast_imp_st* pimp)
{
	te_module_et ty = TE_MODULE_NONE;
	for (size_t i = 0; i < nimpdirs; i++)
		if (pimpdirs[i])
		{
			ty = imp_exists(pimpdirs[i], pimp);
			if (ty)
				break;
		}
	
	pmodule->ty = ty;
	switch (ty)
	{
	case TE_MODULE_NONE:
		return te_seterr("Unable to find path for import: %s", pimp->imp_pth[pimp->length - 1]);
	case TE_MODULE_INV:
		return NULL;
	case TE_MODULE_SCRIPT:
		return load_script(pmodule);
	case TE_MODULE_BIN:
		return load_bin(pmodule);
	}

	assert(false && "imp_exists returned an invalid value");
}

te_scope_st* module_import(te_scope_st* pscope, const te_module_st* pmodule)
{
	switch (pmodule->ty)
	{
	case TE_MODULE_NONE:
		return te_seterr("Unable to import an uninitialized module");
	case TE_MODULE_INV:
		return NULL;
	case TE_MODULE_SCRIPT:
	{
		te_obj_st* pobj = te_eval(pscope, pmodule->past);
		if (te_haserr())
			return NULL;
		return pscope;
	}
	case TE_MODULE_BIN:
		return pmodule->loadmod(pscope);
	default:
		assert(false);
		return te_seterr("Unrecognized module type");
	}
}

void module_close(te_module_st* pmodule)
{
	if (pmodule->ty == TE_MODULE_SCRIPT)
	{
		_te_ast_del(pmodule->past);
		free(pmodule->past);
	}
}
