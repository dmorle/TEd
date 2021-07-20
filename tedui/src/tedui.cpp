#define TEDUI_SRC

#include <tedui/tedui.hpp>
#include <tedcore/tedcore.hpp>

static ted::graphics::Brush* pbrush;
char buf[sizeof(ted::graphics::Rect)];

TEDUI_API void tedui_startup()
{
	pbrush = new ted::graphics::Brush({ 1, 0.5, 1 });
	
	*(ted::graphics::Rect*)buf = ted::graphics::createRect(1.0, pbrush, 100, 100, 200, 200);
}

TEDUI_API void tedui_shutdown()
{
	((ted::graphics::Rect*)buf)->release();
	pbrush->release();
	delete pbrush;
}
