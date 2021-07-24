#include <tedcore/backend/tedlapi/line.hpp>

#define self (*(line*)pself)
#define CHECK_TYPE_RET(ret) if (pself->ty != &line_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_TYPE CHECK_TYPE_RET(NULL)

namespace ted
{
	namespace tedl
	{
		te_type_st line_ty = {
			.name = "line",
			.objsize = sizeof(line),
			.ty_new = &line_new,
			.ty_del = &line_del,
			.ty_bool = &line_bool
		};

		te_obj_st* line_new()
		{
			line* pline = (line*)malloc(sizeof(line));
			if (!pline)
				return (te_obj_st*)te_seterr("Out of memory");

			te_obj_new((te_obj_st*)pline, &line_ty);
			return (te_obj_st*)pline;
		}

		void line_del(te_obj_st* pself)
		{
			CHECK_TYPE_RET();
			te_obj_del(pself);
			if (!pself->is_valid)
				return;

			self.line.release();
		}

		bool line_bool(te_obj_st* pself)
		{
			CHECK_TYPE_RET(false);
			return true;
		}
	}
}
