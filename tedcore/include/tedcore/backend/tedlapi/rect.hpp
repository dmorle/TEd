#ifndef TEDCORE_TEDLAPI_RECT_H
#define TEDCORE_TEDLAPI_RECT_H

#include <tedcore/tedcore.hpp>
#include <tedlang/tedl.h>

namespace ted
{
	namespace tedl
	{
		te_obj_st* rect_new();
		void rect_del(te_obj_st* pself);

		bool rect_bool(te_obj_st* pself);

		extern te_type_st rect_ty;

		struct rect
		{
			te_obj_st super;
			ted::graphics::Rect rect;
		};
	}
}

#endif
