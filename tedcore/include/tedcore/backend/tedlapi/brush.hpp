#ifndef TEDCORE_TEDLAPI_BRUSH_H
#define TEDCORE_TEDLAPI_BRUSH_H

#include <tedcore/tedcore.hpp>
#include <tedlang/tedl.h>

namespace tedl
{
	te_obj_st* brush_new();
	void brush_del(te_obj_st* pself);

	bool brush_bool(te_obj_st* pself);

	extern te_type_st brush_ty;

	struct brush
	{
		te_obj_st super;
		ted::graphics::Brush brush;
	};
}

#endif
