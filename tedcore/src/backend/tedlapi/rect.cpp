#include <tedcore/backend/tedlapi/rect.hpp>

#define self (*(rect*)pself)
#define CHECK_TYPE_RET(ret) if (pself->ty != &rect_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_TYPE CHECK_TYPE_RET(NULL)

namespace tedl
{

}
