#include <string>

#include <tedcore/tedcore.hpp>
#include <tedlang/builtin/null.h>
#include <tedlang/builtin/fn.h>

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

static te_obj_st* reg_startup_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	static bool tedl_dispatch = false;

	if (pargs->argc != 1)
		return (te_obj_st*)te_seterr("Invalid argument number to reg_startup");
	
	if (pargs->ppargs[0]->ty->name == "fn")
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

static te_type_st reg_startup_ty = {
	.name = "reg_startup_fn",
	.objsize = sizeof(te_obj_st),
	.ty_call = &reg_startup_call
};
static te_type_st reg_shutdown_ty = {
	.name = "reg_startup_fn",
	.objsize = sizeof(te_obj_st),
	.ty_call = &reg_shutdown_call
};

static te_obj_st reg_startup_fn = {
	.ty = &reg_startup_ty,
	.n_ref = 1,
	.is_valid = true
};
static te_obj_st reg_shutdown_fn = {
	.ty = &reg_shutdown_ty,
	.n_ref = 1,
	.is_valid = true
};

void ted::impl::buildApi(te_scope_st* pscope)
{
	if (!te_scope_set(pscope, "reg_startup", &reg_startup_fn)) goto MEM_ERR;
	if (!te_scope_set(pscope, "reg_shutdown", &reg_shutdown_fn)) goto MEM_ERR;
	// TODO: create the remaining reg_* functions

MEM_ERR:
	// TODO: clean shutdown
	messageBox("Out of memory");
	exit(1);
}
