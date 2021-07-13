#include <tedcore/backend/tedlapi/rect.hpp>

#define self (*(rect*)pself)
#define CHECK_TYPE_RET(ret) if (pself->ty != &rect_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_TYPE CHECK_TYPE_RET(NULL)

namespace ted
{
	namespace tedl
	{
		te_type_st rect_ty = {
			.name = "rect",
			.objsize = sizeof(rect),
			.ty_new = &rect_new,
			.ty_del = &rect_del,
			.ty_bool = &rect_bool
		};

		te_obj_st* rect_new()
		{
			rect* prect = (rect*)malloc(sizeof(rect));
			if (!prect)
				return (te_obj_st*)te_seterr("Out of memory");

			te_obj_new((te_obj_st*)prect, &rect_ty);
			return (te_obj_st*)prect;
		}

		void rect_del(te_obj_st* pself)
		{
			CHECK_TYPE_RET();
			te_obj_del(pself);
			if (!pself->is_valid)
				return;

			self.rect.release();
		}

		bool rect_bool(te_obj_st* pself)
		{
			CHECK_TYPE_RET(false);
			return true;
		}
	}
}
