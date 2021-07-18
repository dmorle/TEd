#define TEDCORE_SRC

#include <mutex>
#include <filesystem>

#include <tedlang/tedl.h>
#include <tedcore/tedcore.hpp>
#include <tedcore/backend/impl.hpp>

namespace ted
{
	namespace events
	{
		TEDCORE_API std::vector<basicHandler> startupHandlers    = {};
		TEDCORE_API std::vector<basicHandler> shutdownHandlers   = {};
		TEDCORE_API std::vector<basicHandler> minimizeHandlers   = {};
		TEDCORE_API std::vector<sizeHandler > maximizeHandlers   = {};
		TEDCORE_API std::vector<sizeHandler > winSizeHandlers    = {};
		TEDCORE_API std::vector<posHandler  > winMoveHandlers    = {};
		TEDCORE_API std::vector<keyHandler  > keyDownHandlers    = {};
		TEDCORE_API std::vector<keyHandler  > keyUpHandlers      = {};
		TEDCORE_API std::vector<posHandler  > mouseLDownHandlers = {};
		TEDCORE_API std::vector<posHandler  > mouseLUpHandlers   = {};
		TEDCORE_API std::vector<posHandler  > mouseMDownHandlers = {};
		TEDCORE_API std::vector<posHandler  > mouseMUpHandlers   = {};
		TEDCORE_API std::vector<posHandler  > mouseRDownHandlers = {};
		TEDCORE_API std::vector<posHandler  > mouseRUpHandlers   = {};
		TEDCORE_API std::vector<posHandler  > mouseMoveHandlers  = {};
		TEDCORE_API std::vector<wheelHandler> mouseWheelHandlers = {};
	}

	namespace impl
	{
		static te_scope_st* pscope;

		TEDCORE_API void init()
		{
			namespace fs = std::filesystem;

			if (!render_buf::rb_ptr())
			{
				messageBox("Out of memory");
				exit(1);
			}

			// constructing the api
			pscope = te_init();
			if (!pscope)
			{
				messageBox("Out of memory");
				exit(1);
			}
			buildApi(pscope);
			
			fs::path extdir = "extensions";
			if (!fs::is_directory(extdir))
				return;

			// loading tedcore extensions
			for (auto& p : fs::directory_iterator(extdir))
			{
				if (!p.is_regular_file())
					continue;
				fs::path ext = p.path().extension();
				te_module_st mod;
				te_module_st* pret;

				std::string extstr = p.path().string();
				const char* extcstr = extstr.c_str();
				if (ext == ".ted")
					pret = module_load_script(&mod, extcstr);
				else if (ext == ".dll")
					pret = module_load_bin(&mod, extcstr);
				else
					continue;
				if (!pret)
				{
					messageBox("Error loading module: " + extstr);
					exit(1);
				}
				if (!module_import(pscope, &mod))
				{
					messageBox("Error importing module: " + extstr);
					exit(1);
				}
				module_close(&mod);
			}
		}

		namespace render_buf
		{
			TEDCORE_API ID2D1HwndRenderTarget* pRenderTarget = NULL;

			static size_t rb_end = 0;
			static size_t rb_memsz = 256;
			static size_t rb_elemnum = 0;
			static uint8_t* rb_buf = (uint8_t*)malloc(rb_memsz);
			static std::mutex rb_mtx;

			TEDCORE_API void* rb_malloc(float depth, size_t sz)
			{
				while (sz >= rb_memsz - rb_end)
				{
					size_t nsz = rb_memsz + rb_memsz;
					void* nrb_buf = std::realloc(rb_buf, nsz);
					if (!nrb_buf)
						throw std::bad_alloc();
					rb_buf = (uint8_t*)nrb_buf;
					rb_memsz = nsz;
				}

				uint8_t* prb = rb_buf;
				size_t idx = 0;
				while (idx < rb_elemnum && depth > ((RBEHead*)prb)->depth)
					prb += ((RBEHead*)prb)->elemsz;

				if (idx < rb_elemnum)
					// making room at prb
					std::memmove(prb + sz, prb, rb_end - (prb - rb_buf));
				rb_end += sz;
				rb_elemnum++;
				return prb;
			}

			TEDCORE_API void rb_free(rb_handle handle)
			{
				uint8_t* prb = rb_buf;
				while (((RBEHead*)prb)->handle != handle)
					prb += ((RBEHead*)prb)->elemsz;
				((RBEHead*)prb)->id = type_id_map::get_id<EmptyDef>();
			}

			TEDCORE_API void* rb_ptr()
			{
				return rb_buf;
			}

			TEDCORE_API size_t rb_count()
			{
				return rb_elemnum;
			}

			TEDCORE_API void rb_pack()
			{
				// TODO: Implement a packing algorithm
			}
		}
	}
}
