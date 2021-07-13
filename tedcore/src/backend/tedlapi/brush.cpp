#include <tedcore/backend/tedlapi/brush.hpp>

#define self (*(brush*)pself)
#define CHECK_TYPE_RET()    if (pself->ty != &brush_ty) { te_seterr("Invalid Type"); return;     }
#define CHECK_TYPE_RET(ret) if (pself->ty != &brush_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_TYPE CHECK_TYPE_RET(NULL)

namespace ted
{
	namespace tedl
	{
		te_type_st brush_ty = {
			.name = "brush",
			.objsize = sizeof(brush),
			.ty_new = &brush_new,
			.ty_del = &brush_del,
			.ty_bool = &brush_bool
		};

		te_obj_st* brush_new()
		{
			brush* pbrush = (brush*)malloc(sizeof(brush));
			if (!pbrush)
				return (te_obj_st*)te_seterr("Out of memory");

			te_obj_new((te_obj_st*)pbrush, &brush_ty);
			return (te_obj_st*)pbrush;
		}

		void brush_del(te_obj_st* pself)
		{
			CHECK_TYPE_RET();
			te_obj_del(pself);
			if (!pself->is_valid)
				return;

			self.brush.release();
		}

		bool brush_bool(te_obj_st* pself)
		{
			CHECK_TYPE_RET(false);
			return true;
		}
	}
}
