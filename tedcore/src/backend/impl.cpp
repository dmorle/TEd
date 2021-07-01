#define TEDCORE_SRC

#include <tedlang/tedl.h>
#include <tedcore/tedcore.hpp>

namespace ted
{
	namespace events
	{
		TEDCORE_API std::vector<void(*)()> startupHandlers   ;
		TEDCORE_API std::vector<void(*)()> shutdownHandlers  ;
		TEDCORE_API std::vector<void(*)()> minimizeHandlers  ;
		TEDCORE_API std::vector<void(*)()> maximizeHandlers  ;
		TEDCORE_API std::vector<void(*)()> winSizeHandlers   ;
		TEDCORE_API std::vector<void(*)()> winMoveHandlers   ;
		TEDCORE_API std::vector<void(*)(key)> keyDownHandlers;
		TEDCORE_API std::vector<void(*)(key)> keyUpHandlers  ;
		TEDCORE_API std::vector<void(*)()> mouseLDownHandlers;
		TEDCORE_API std::vector<void(*)()> mouseLUpHandlers  ;
		TEDCORE_API std::vector<void(*)()> mouseMDownHandlers;
		TEDCORE_API std::vector<void(*)()> mouseMUpHandlers  ;
		TEDCORE_API std::vector<void(*)()> mouseRDownHandlers;
		TEDCORE_API std::vector<void(*)()> mouseRUpHandlers  ;
		TEDCORE_API std::vector<void(*)()> mouseMoveHandlers ;
	}

	namespace impl
	{
		static te_scope_st* pscope;

		TEDCORE_API void init()
		{
			pscope = te_init();
			if (!pscope)
			{
				messageBox("Out of memory");
				exit(1);
			}

			buildApi(pscope);
			
			
		}

		namespace render_buf
		{
			static float rbuf_lf = 1.0;
			static size_t buf_idx = 0;
			static size_t mem_sz = 256;
			void* buf = malloc(mem_sz);

		}
	}
}
