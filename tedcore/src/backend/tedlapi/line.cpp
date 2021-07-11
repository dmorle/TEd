#include <tedcore/backend/tedlapi/line.hpp>

#define self (*(line*)pself)
#define CHECK_TYPE_RET(ret) if (pself->ty != &line_ty) { te_seterr("Invalid Type"); return ret; }
#define CHECK_TYPE CHECK_TYPE_RET(NULL)

namespace tedl
{

}
