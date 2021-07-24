#ifndef TEDCORE_TEDLAPI_LINE_H
#define TEDCORE_TEDLAPI_LINE_H

#include <tedcore/tedcore.hpp>
#include <tedlang/tedl.h>

namespace ted
{
	namespace tedl
	{
		te_obj_st* line_new();
		void line_del(te_obj_st* pself);

		bool line_bool(te_obj_st* pself);

		extern te_type_st line_ty;

		struct line
		{
			te_obj_st super;
			ted::graphics::GLine line;
		};
	}
}

#endif
