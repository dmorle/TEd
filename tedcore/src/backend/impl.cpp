#include <tedcore/backend/impl.hpp>

namespace ted
{
	namespace impl
	{
		namespace render_buf
		{
			static float rbuf_lf = 1.0;
			static size_t buf_idx = 0;
			static size_t mem_sz = 256;
			void* buf = malloc(mem_sz);

		}
	}
}
