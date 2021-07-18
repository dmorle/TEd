#define TEDCORE_SRC

#include <string>
#include <bitset>

#include <tedlang/tedl.h>
#include <tedcore/tedcore.hpp>

#include <tedcore/backend/tedlapi/brush.hpp>
#include <tedcore/backend/tedlapi/line.hpp>
#include <tedcore/backend/tedlapi/rect.hpp>

#include <tedslib/os/cptr.h>

static std::vector<te_obj_st*> tedl_startups  = {};
static std::vector<te_obj_st*> tedl_shutdowns = {};
static std::vector<te_obj_st*> tedl_minimizes = {};
static std::vector<te_obj_st*> tedl_maximizes = {};
static std::vector<te_obj_st*> tedl_wsizes    = {};
static std::vector<te_obj_st*> tedl_wmoves    = {};
static std::vector<te_obj_st*> tedl_keydowns  = {};
static std::vector<te_obj_st*> tedl_keyups    = {};
static std::vector<te_obj_st*> tedl_ldowns    = {};
static std::vector<te_obj_st*> tedl_lups      = {};
static std::vector<te_obj_st*> tedl_mdowns    = {};
static std::vector<te_obj_st*> tedl_mups      = {};
static std::vector<te_obj_st*> tedl_rdowns    = {};
static std::vector<te_obj_st*> tedl_rups      = {};
static std::vector<te_obj_st*> tedl_mmoves    = {};
static std::vector<te_obj_st*> tedl_mwheels   = {};

static std::bitset<16> tedl_flags;

template<std::vector<te_obj_st*>& tedl_handlers>
consteval uint8_t get_bit()
{
	if (&tedl_handlers == &tedl_startups  ) return 0x0;
	if (&tedl_handlers == &tedl_shutdowns ) return 0x1;
	if (&tedl_handlers == &tedl_minimizes ) return 0x2;
	if (&tedl_handlers == &tedl_maximizes ) return 0x3;
	if (&tedl_handlers == &tedl_wsizes    ) return 0x4;
	if (&tedl_handlers == &tedl_wmoves    ) return 0x5;
	if (&tedl_handlers == &tedl_keydowns  ) return 0x6;
	if (&tedl_handlers == &tedl_keyups    ) return 0x7;
	if (&tedl_handlers == &tedl_ldowns    ) return 0x8;
	if (&tedl_handlers == &tedl_lups      ) return 0x9;
	if (&tedl_handlers == &tedl_mdowns    ) return 0xA;
	if (&tedl_handlers == &tedl_mups      ) return 0xB;
	if (&tedl_handlers == &tedl_rdowns    ) return 0xC;
	if (&tedl_handlers == &tedl_rups      ) return 0xD;
	if (&tedl_handlers == &tedl_mmoves    ) return 0xE;
	if (&tedl_handlers == &tedl_mwheels   ) return 0xF;
}

template<std::vector<te_obj_st*>& tedl_handlers>
static inline void call_handlers(const te_fnargs_st* pargs)
{
	te_obj_st* pret;
	for (auto e : tedl_handlers)
	{
		pret = te_call(e, pargs);
		if (te_haserr())
		{
			ted::messageBox("Error occured in a tedlang handler");
			// TODO: dump the traceback, and find a better way of dealing with these failures
			exit(1);
		}
		te_decref_s(pret);
	}
}
template<std::vector<te_obj_st*>& tedl_handlers>
void dispatcher()
{
	te_fnargs_st fnargs = { 0, NULL };
	call_handlers<tedl_handlers>(&fnargs);
}

template<std::vector<te_obj_st*>& tedl_handlers>
static void dispatcher(uint16_t arg1, uint16_t arg2)
{
	te_int_st* parg1 = (te_int_st*)te_int_new();
	if (te_haserr())
	{
		ted::messageBox("Critical error occured in a tedlang dispatcher");
		exit(1);
	}
	te_int_st* parg2 = (te_int_st*)te_int_new();
	if (te_haserr())
	{
		ted::messageBox("Critical error occured in a tedlang dispatcher");
		exit(1);
	}

	parg1->val = arg1;
	parg2->val = arg2;
	te_obj_st* pargs[2] = { (te_obj_st*)parg1, (te_obj_st*)parg2 };
	te_fnargs_st fnargs = { 2, pargs };
	call_handlers<tedl_handlers>(&fnargs);
}

template<std::vector<te_obj_st*>& tedl_handlers>
static void dispatcher(ted::events::key arg)
{
	te_int_st* parg = (te_int_st*)te_int_new();
	if (te_haserr())
	{
		ted::messageBox("Critical error occured in a tedlang dispatcher");
		exit(1);
	}
	parg->val = arg;
	te_obj_st* pargs[1] = { (te_obj_st*)parg };
	te_fnargs_st fnargs = { 1, pargs };
	call_handlers<tedl_handlers>(&fnargs);
}

template<std::vector<te_obj_st*>& tedl_handlers>
static void dispatcher(int16_t arg)
{
	te_int_st* parg = (te_int_st*)te_int_new();
	if (te_haserr())
	{
		ted::messageBox("Critical error occured in a tedlang dispatcher");
		exit(1);
	}
	parg->val = arg;
	te_obj_st* pargs[1] = { (te_obj_st*)parg };
	te_fnargs_st fnargs = { 1, pargs };
	call_handlers<tedl_handlers>(&fnargs);
}

//
// tedl api call functions
//

static te_obj_st* message_box_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	if (pargs->argc != 1)
		return (te_obj_st*)te_seterr("Invalid argument number to message_box");

	if (strcmp(pargs->ppargs[0]->ty->name, "str"))
		return (te_obj_st*)te_seterr("Invalid argument type to message_box");

	ted::messageBox(((te_str_st*)pargs->ppargs[0])->val);
	return te_null_new();
}

template<typename H, std::vector<H>& handler, std::vector<te_obj_st*>& tedl_handler>
static te_obj_st* reg_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	if (pargs->argc != 1)
		return (te_obj_st*)te_seterr("Invalid argument number to reg_call");

	if (!strcmp(pargs->ppargs[0]->ty->name, "fn"))
	{
		if (!tedl_flags[get_bit<tedl_handler>()])
			handler.push_back(dispatcher<tedl_handler>);
		tedl_flags[get_bit<tedl_handler>()] = true;
		te_incref(pargs->ppargs[0]);
		tedl_handler.push_back(pargs->ppargs[0]);
	}
	else if (!strcmp(pargs->ppargs[0]->ty->name, "cptr"))
		handler.push_back((H)((te_cptr_st*)pargs->ppargs[0])->ptr);
	else
		return (te_obj_st*)te_seterr("Invalid argument type to reg_call");

	return te_null_new();
}

using te_fn = te_obj_st * (*)(te_obj_st*, const te_fnargs_st*);

static te_fn reg_startup_call  = reg_call< ted::events::basicHandler, ted::events::startupHandlers   , tedl_startups  >;
static te_fn reg_shutdown_call = reg_call< ted::events::basicHandler, ted::events::shutdownHandlers  , tedl_shutdowns >;
static te_fn reg_maximize_call = reg_call< ted::events::basicHandler, ted::events::minimizeHandlers  , tedl_minimizes >;
static te_fn reg_minimize_call = reg_call< ted::events::sizeHandler , ted::events::maximizeHandlers  , tedl_maximizes >;
static te_fn reg_wsize_call    = reg_call< ted::events::sizeHandler , ted::events::winSizeHandlers   , tedl_wsizes    >;
static te_fn reg_wmove_call    = reg_call< ted::events::posHandler  , ted::events::winMoveHandlers   , tedl_wmoves    >;
static te_fn reg_keydown_call  = reg_call< ted::events::keyHandler  , ted::events::keyDownHandlers   , tedl_keydowns  >;
static te_fn reg_keyup_call    = reg_call< ted::events::keyHandler  , ted::events::keyUpHandlers     , tedl_keyups    >;
static te_fn reg_ldown_call    = reg_call< ted::events::posHandler  , ted::events::mouseLDownHandlers, tedl_ldowns    >;
static te_fn reg_lup_call      = reg_call< ted::events::posHandler  , ted::events::mouseLUpHandlers  , tedl_lups      >;
static te_fn reg_mdown_call    = reg_call< ted::events::posHandler  , ted::events::mouseMDownHandlers, tedl_mdowns    >;
static te_fn reg_mup_call      = reg_call< ted::events::posHandler  , ted::events::mouseMUpHandlers  , tedl_mups      >;
static te_fn reg_rdown_call    = reg_call< ted::events::posHandler  , ted::events::mouseRDownHandlers, tedl_rdowns    >;
static te_fn reg_rup_call      = reg_call< ted::events::posHandler  , ted::events::mouseRUpHandlers  , tedl_rups      >;
static te_fn reg_mmove_call    = reg_call< ted::events::posHandler  , ted::events::mouseMoveHandlers , tedl_mmoves    >;
static te_fn reg_mwheel_call   = reg_call< ted::events::wheelHandler, ted::events::mouseWheelHandlers, tedl_mwheels   >;

static te_obj_st* create_brush_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	if (pargs->argc != 3)
		return (te_obj_st*)te_seterr("Function argument mismatch: expected 3, recieved %zu", pargs->argc);

	for (int i = 0; i < 3; i++)
		if (strcmp(pargs->ppargs[i]->ty->name, "int"))
			return (te_obj_st*)te_seterr("Invalid argument type, expected int");

	ted::graphics::Color c;
	c.r = (float)((te_int_st*)pargs->ppargs[0])->val / 255;
	c.g = (float)((te_int_st*)pargs->ppargs[1])->val / 255;
	c.b = (float)((te_int_st*)pargs->ppargs[2])->val / 255;

	ted::tedl::brush* pbrush = (ted::tedl::brush*)ted::tedl::brush_new();
	if (te_haserr())
		return NULL;

	pbrush->brush = ted::graphics::Brush(c);
	return (te_obj_st*)pbrush;
}

static te_obj_st* create_line_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	if (pargs->argc != 7)
		return (te_obj_st*)te_seterr("Function argument mismatch: expected 7, recieved %zu", pargs->argc);

	if (strcmp(pargs->ppargs[0]->ty->name, "float")) return (te_obj_st*)te_seterr("Invalid argument type, expected float");
	if (strcmp(pargs->ppargs[1]->ty->name, "brush")) return (te_obj_st*)te_seterr("Invalid argument type, expected brush");
	if (strcmp(pargs->ppargs[2]->ty->name, "int"))   return (te_obj_st*)te_seterr("Invalid argument type, expected int"  );
	if (strcmp(pargs->ppargs[3]->ty->name, "int"))   return (te_obj_st*)te_seterr("Invalid argument type, expected int"  );
	if (strcmp(pargs->ppargs[4]->ty->name, "int"))   return (te_obj_st*)te_seterr("Invalid argument type, expected int"  );
	if (strcmp(pargs->ppargs[5]->ty->name, "int"))   return (te_obj_st*)te_seterr("Invalid argument type, expected int"  );
	if (strcmp(pargs->ppargs[5]->ty->name, "float")) return (te_obj_st*)te_seterr("Invalid argument type, expected float");

	ted::tedl::line* pline = (ted::tedl::line*)ted::tedl::line_new();
	if (te_haserr())
		return NULL;

	ted::Point p1 = {
		(float)((te_int_st*)pargs->ppargs[2])->val,
		(float)((te_int_st*)pargs->ppargs[3])->val
	};

	ted::Point p2 = {
		(float)((te_int_st*)pargs->ppargs[5])->val,
		(float)((te_int_st*)pargs->ppargs[6])->val
	};

	pline->line = ted::graphics::createLine(
		((te_float_st*)pargs->ppargs[0])->val,
		&((ted::tedl::brush*)pargs->ppargs[1])->brush,
		p1, p2,
		((te_float_st*)pargs->ppargs[6])->val
	);

	return (te_obj_st*)pline;
}

static te_obj_st* create_rect_call(te_obj_st* pself, const te_fnargs_st* pargs)
{
	if (pargs->argc != 6)
		return (te_obj_st*)te_seterr("Function argument mismatch: expected 6, recieved %zu", pargs->argc);

	if (strcmp(pargs->ppargs[0]->ty->name, "float")) return (te_obj_st*)te_seterr("Invalid argument type, expected float");
	if (strcmp(pargs->ppargs[1]->ty->name, "brush")) return (te_obj_st*)te_seterr("Invalid argument type, expected brush");
	if (strcmp(pargs->ppargs[2]->ty->name, "int"))   return (te_obj_st*)te_seterr("Invalid argument type, expected int"  );
	if (strcmp(pargs->ppargs[3]->ty->name, "int"))   return (te_obj_st*)te_seterr("Invalid argument type, expected int"  );
	if (strcmp(pargs->ppargs[4]->ty->name, "int"))   return (te_obj_st*)te_seterr("Invalid argument type, expected int"  );
	if (strcmp(pargs->ppargs[5]->ty->name, "int"))   return (te_obj_st*)te_seterr("Invalid argument type, expected int"  );

	ted::tedl::rect* prect = (ted::tedl::rect*)ted::tedl::rect_new();
	if (te_haserr())
		return NULL;

	prect->rect = ted::graphics::createRect(
		((te_float_st*)pargs->ppargs[0])->val,
		&((ted::tedl::brush*)pargs->ppargs[1])->brush,
		(float)((te_int_st*)pargs->ppargs[2])->val,
		(float)((te_int_st*)pargs->ppargs[3])->val,
		(float)((te_int_st*)pargs->ppargs[4])->val,
		(float)((te_int_st*)pargs->ppargs[5])->val
	);

	return (te_obj_st*)prect;
}

//
// tedl api types
//

static te_type_st message_box_ty  = { .name = "message_box_fn",  .objsize = sizeof(te_obj_st), .ty_call = message_box_call  };

static te_type_st reg_startup_ty  = { .name = "reg_startup_fn",  .objsize = sizeof(te_obj_st), .ty_call = reg_startup_call  };
static te_type_st reg_shutdown_ty = { .name = "reg_shutdown_fn", .objsize = sizeof(te_obj_st), .ty_call = reg_shutdown_call };
static te_type_st reg_maximize_ty = { .name = "reg_maximize_fn", .objsize = sizeof(te_obj_st), .ty_call = reg_maximize_call };
static te_type_st reg_minimize_ty = { .name = "reg_minimize_fn", .objsize = sizeof(te_obj_st), .ty_call = reg_minimize_call };
static te_type_st reg_wsize_ty    = { .name = "reg_wsize_fn",    .objsize = sizeof(te_obj_st), .ty_call = reg_wsize_call    };
static te_type_st reg_wmove_ty    = { .name = "reg_wmove_fn",    .objsize = sizeof(te_obj_st), .ty_call = reg_wmove_call    };
static te_type_st reg_keydown_ty  = { .name = "reg_keydown_fn",  .objsize = sizeof(te_obj_st), .ty_call = reg_keydown_call  };
static te_type_st reg_keyup_ty    = { .name = "reg_keyup_fn",    .objsize = sizeof(te_obj_st), .ty_call = reg_keyup_call    };
static te_type_st reg_ldown_ty    = { .name = "reg_ldown_fn",    .objsize = sizeof(te_obj_st), .ty_call = reg_ldown_call    };
static te_type_st reg_lup_ty      = { .name = "reg_lup_fn",      .objsize = sizeof(te_obj_st), .ty_call = reg_lup_call      };
static te_type_st reg_mdown_ty    = { .name = "reg_mdown_fn",    .objsize = sizeof(te_obj_st), .ty_call = reg_mdown_call    };
static te_type_st reg_mup_ty      = { .name = "reg_mup_fn",      .objsize = sizeof(te_obj_st), .ty_call = reg_mup_call      };
static te_type_st reg_rdown_ty    = { .name = "reg_rdown_fn",    .objsize = sizeof(te_obj_st), .ty_call = reg_rdown_call    };
static te_type_st reg_rup_ty      = { .name = "reg_rup_fn",      .objsize = sizeof(te_obj_st), .ty_call = reg_rup_call      };
static te_type_st reg_mmove_ty    = { .name = "reg_mmove_fn",    .objsize = sizeof(te_obj_st), .ty_call = reg_mmove_call    };
static te_type_st reg_mwheel_ty   = { .name = "reg_mwheel_fn",   .objsize = sizeof(te_obj_st), .ty_call = reg_mwheel_call   };

static te_type_st create_brush_ty = { .name = "create_brush_fn", .objsize = sizeof(te_obj_st), .ty_call = create_brush_call };
static te_type_st create_line_ty  = { .name = "create_line_fn",  .objsize = sizeof(te_obj_st), .ty_call = create_line_call  };
static te_type_st create_rect_ty  = { .name = "create_rect_fn",  .objsize = sizeof(te_obj_st), .ty_call = create_rect_call  };

//
// tedl api objects
//

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
static te_obj_st reg_mwheel_fn   = { .ty = &reg_mwheel_ty,   .n_ref = 1, .is_valid = true };

static te_obj_st create_brush_fn = { .ty = &create_brush_ty, .n_ref = 1, .is_valid = true };
static te_obj_st create_rect_fn  = { .ty = &create_rect_ty,  .n_ref = 1, .is_valid = true };
static te_obj_st create_line_fn  = { .ty = &create_line_ty,  .n_ref = 1, .is_valid = true };

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
	if (!te_scope_set(pscope, "reg_mwheel",   &reg_mwheel_fn))   goto MEM_ERR;

	if (!te_scope_set(pscope, "create_brush", &create_brush_fn)) goto MEM_ERR;
	if (!te_scope_set(pscope, "create_rect",  &create_rect_fn))  goto MEM_ERR;
	if (!te_scope_set(pscope, "create_line",  &create_line_fn))  goto MEM_ERR;

	return;

MEM_ERR:
	// TODO: clean shutdown
	messageBox("Out of memory");
	exit(1);
}
