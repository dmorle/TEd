#define TEDCORE_SRC

#include <string>

#include <tedlang/tedl.h>
#include <tedcore/tedcore.hpp>

std::vector<te_obj_st*> tedl_startups = {};
void tedl_startup_dispatch()
{
	te_fnargs_st fnargs = {
		0,
		NULL
	};
	te_obj_st* pret;
	for (auto e : tedl_startups)
	{
		pret = te_call(e, &fnargs);
		if (te_haserr())
		{
			ted::messageBox("Error occured on tedlang startup handler");
			// TODO: dump the traceback, and find a better way of dealing with these failures
			exit(1);
		}
		te_decref_s(pret);
	}
}

std::vector<te_obj_st*> tedl_shutdowns = {};
void tedl_shutdown_dispatch()
{
	te_fnargs_st fnargs = {
		0,
		NULL
	};
	te_obj_st* pret;
	for (auto e : tedl_shutdowns)
	{
		pret = te_call(e, &fnargs);
		if (te_haserr())
		{
			ted::messageBox("Error occured on tedlang shutdown handler");
			exit(1);
		}
		te_decref_s(pret);
	}
}

static te_obj_st* message_box_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	if (pargs->argc != 1)
		return (te_obj_st*)te_seterr("Invalid argument number to message_box");

	if (strcmp(pargs->ppargs[0]->ty->name, "str"))
		return (te_obj_st*)te_seterr("Invalid argument type to message_box");

	ted::messageBox(((te_str_st*)pargs->ppargs[0])->val);
	return te_null_new();
}

static te_obj_st* reg_startup_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	static bool tedl_dispatch = false;

	if (pargs->argc != 1)
		return (te_obj_st*)te_seterr("Invalid argument number to reg_startup");
	
	if (!strcmp(pargs->ppargs[0]->ty->name, "fn"))
	{
		if (!tedl_dispatch)
			ted::events::startupHandlers.push_back(&tedl_startup_dispatch);
		tedl_dispatch = true;
		te_incref(pargs->ppargs[0]);
		tedl_startups.push_back(pargs->ppargs[0]);
	}
	// TODO: Check for C/C++ function pointer argument type
	else
		return (te_obj_st*)te_seterr("Invalid argument type to reg_startup");

	return te_null_new();
}

static te_obj_st* reg_shutdown_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	return te_null_new();
}

static te_obj_st* reg_maximize_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	return te_null_new();
}

static te_obj_st* reg_minimize_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	return te_null_new();
}

static te_obj_st* reg_wsize_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	return te_null_new();
}

static te_obj_st* reg_wmove_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	return te_null_new();
}

static te_obj_st* reg_keydown_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	return te_null_new();
}

static te_obj_st* reg_keyup_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	return te_null_new();
}

static te_obj_st* reg_ldown_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	return te_null_new();
}

static te_obj_st* reg_lup_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	return te_null_new();
}

static te_obj_st* reg_mdown_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	return te_null_new();
}

static te_obj_st* reg_mup_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	return te_null_new();
}

static te_obj_st* reg_rdown_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	return te_null_new();
}

static te_obj_st* reg_rup_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	return te_null_new();
}

static te_obj_st* reg_mmove_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	return te_null_new();
}

static te_type_st message_box_ty  = { .name = "message_box_fn",  .objsize = sizeof(te_obj_st), .ty_call = &message_box_call };
static te_type_st reg_startup_ty  = { .name = "reg_startup_fn",  .objsize = sizeof(te_obj_st), .ty_call = &reg_startup_call };
static te_type_st reg_shutdown_ty = { .name = "reg_shutdown_fn", .objsize = sizeof(te_obj_st), .ty_call = &reg_shutdown_call };
static te_type_st reg_maximize_ty = { .name = "reg_maximize_fn", .objsize = sizeof(te_obj_st), .ty_call = &reg_maximize_call };
static te_type_st reg_minimize_ty = { .name = "reg_minimize_fn", .objsize = sizeof(te_obj_st), .ty_call = &reg_minimize_call };
static te_type_st reg_wsize_ty    = { .name = "reg_wsize_fn",    .objsize = sizeof(te_obj_st), .ty_call = &reg_wsize_call };
static te_type_st reg_wmove_ty    = { .name = "reg_wmove_fn",    .objsize = sizeof(te_obj_st), .ty_call = &reg_wmove_call };
static te_type_st reg_keydown_ty  = { .name = "reg_keydown_fn",  .objsize = sizeof(te_obj_st), .ty_call = &reg_keydown_call };
static te_type_st reg_keyup_ty    = { .name = "reg_keyup_fn",    .objsize = sizeof(te_obj_st), .ty_call = &reg_keyup_call };
static te_type_st reg_ldown_ty    = { .name = "reg_ldown_fn",    .objsize = sizeof(te_obj_st), .ty_call = &reg_ldown_call };
static te_type_st reg_lup_ty      = { .name = "reg_lup_fn",      .objsize = sizeof(te_obj_st), .ty_call = &reg_lup_call };
static te_type_st reg_mdown_ty    = { .name = "reg_mdown_fn",    .objsize = sizeof(te_obj_st), .ty_call = &reg_mdown_call };
static te_type_st reg_mup_ty      = { .name = "reg_mup_fn",      .objsize = sizeof(te_obj_st), .ty_call = &reg_mup_call };
static te_type_st reg_rdown_ty    = { .name = "reg_rdown_fn",    .objsize = sizeof(te_obj_st), .ty_call = &reg_rdown_call };
static te_type_st reg_rup_ty      = { .name = "reg_rup_fn",      .objsize = sizeof(te_obj_st), .ty_call = &reg_rup_call };
static te_type_st reg_mmove_ty    = { .name = "reg_mmove_fn",    .objsize = sizeof(te_obj_st), .ty_call = &reg_mmove_call };

static te_obj_st message_box_fn  = { .ty = &message_box_ty,  .n_ref = 1, .is_valid = true };
static te_obj_st reg_startup_fn  = { .ty = &reg_startup_ty,  .n_ref = 1, .is_valid = true };
static te_obj_st reg_shutdown_fn = { .ty = &reg_shutdown_ty, .n_ref = 1, .is_valid = true };
static te_obj_st reg_maximize_fn = { .ty = &reg_maximize_ty, .n_ref = 1, .is_valid = true };
static te_obj_st reg_minimize_fn = { .ty = &reg_minimize_ty, .n_ref = 1, .is_valid = true };
static te_obj_st reg_wsize_fn    = { .ty = &reg_wsize_ty,    .n_ref = 1, .is_valid = true };
static te_obj_st reg_wmove_fn    = { .ty = &reg_wmove_ty,    .n_ref = 1, .is_valid = true };
static te_obj_st reg_keydown_fn  = { .ty = &reg_keydown_ty,  .n_ref = 1, .is_valid = true };
static te_obj_st reg_keyup_fn    = { .ty = &reg_keyup_ty,    .n_ref = 1, .is_valid = true };
static te_obj_st reg_ldown_fn    = { .ty = &reg_ldown_ty,    .n_ref = 1, .is_valid = true };
static te_obj_st reg_lup_fn      = { .ty = &reg_lup_ty,      .n_ref = 1, .is_valid = true };
static te_obj_st reg_mdown_fn    = { .ty = &reg_mdown_ty,    .n_ref = 1, .is_valid = true };
static te_obj_st reg_mup_fn      = { .ty = &reg_mup_ty,      .n_ref = 1, .is_valid = true };
static te_obj_st reg_rdown_fn    = { .ty = &reg_rdown_ty,    .n_ref = 1, .is_valid = true };
static te_obj_st reg_rup_fn      = { .ty = &reg_rup_ty,      .n_ref = 1, .is_valid = true };
static te_obj_st reg_mmove_fn    = { .ty = &reg_mmove_ty,    .n_ref = 1, .is_valid = true };

void ted::impl::buildApi(te_scope_st* pscope)
{
	if (!te_scope_set(pscope, "message_box",  &message_box_fn))  goto MEM_ERR;
	if (!te_scope_set(pscope, "reg_startup",  &reg_startup_fn))  goto MEM_ERR;
	if (!te_scope_set(pscope, "reg_shutdown", &reg_shutdown_fn)) goto MEM_ERR;
	if (!te_scope_set(pscope, "reg_maximize", &reg_maximize_fn)) goto MEM_ERR;
	if (!te_scope_set(pscope, "reg_minimize", &reg_minimize_fn)) goto MEM_ERR;
	if (!te_scope_set(pscope, "reg_wsize",    &reg_wsize_fn))    goto MEM_ERR;
	if (!te_scope_set(pscope, "reg_wmove",    &reg_wmove_fn))    goto MEM_ERR;
	if (!te_scope_set(pscope, "reg_keydown",  &reg_keydown_fn))  goto MEM_ERR;
	if (!te_scope_set(pscope, "reg_keyup",    &reg_keyup_fn))    goto MEM_ERR;
	if (!te_scope_set(pscope, "reg_ldown",    &reg_ldown_fn))    goto MEM_ERR;
	if (!te_scope_set(pscope, "reg_lup",      &reg_lup_fn))      goto MEM_ERR;
	if (!te_scope_set(pscope, "reg_mdown",    &reg_mdown_fn))    goto MEM_ERR;
	if (!te_scope_set(pscope, "reg_mup",      &reg_mup_fn))      goto MEM_ERR;
	if (!te_scope_set(pscope, "reg_rdown",    &reg_rdown_fn))    goto MEM_ERR;
	if (!te_scope_set(pscope, "reg_rup",      &reg_rup_fn))      goto MEM_ERR;
	if (!te_scope_set(pscope, "reg_mmove",    &reg_mmove_fn))    goto MEM_ERR;
	return;

MEM_ERR:
	// TODO: clean shutdown
	messageBox("Out of memory");
	exit(1);
}
