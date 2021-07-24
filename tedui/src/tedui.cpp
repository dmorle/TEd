#define TEDUI_SRC

#include <tedui/tedui.hpp>
#include <tedcore/tedcore.hpp>

namespace tedui
{
	namespace brushes
	{
		ted::graphics::Brush* pback      = NULL;
		ted::graphics::Brush* pbase      = NULL;
		ted::graphics::Brush* pselect    = NULL;
		ted::graphics::Brush* pactive    = NULL;
		ted::graphics::Brush* phighlight = NULL;
		ted::graphics::Brush* pfocus     = NULL;
	}

	static BrushMap brushMap = {};

	const BrushMap& getBrushMap()
	{
		return brushMap;
	}

	ted::graphics::Brush& createBrush(const std::string& name, const ted::graphics::Color& c)
	{
		auto [it, success] = brushMap.insert({ name, c });
		// change the color if the brush already exists
		if (!success)
			it->second.setColor(c);
		return it->second;
	}
}

uint8_t ui_rects[1 * sizeof(ted::graphics::GRect)];
#define back_rect     ((ted::graphics::GRect*)ui_rects)[0]

TEDUI_HANDLER void tedui_startup()
{
	tedui::brushes::pback      = &tedui::createBrush("background", { 0.1, 0.1, 0.5 });
	tedui::brushes::pbase      = &tedui::createBrush("base",       { 0.2, 0.2, 0.2 });
	tedui::brushes::pselect    = &tedui::createBrush("select",     { 0.4, 0.4, 0.4 });
	tedui::brushes::pactive    = &tedui::createBrush("active",     { 0.5, 0.5, 0.5 });
	tedui::brushes::phighlight = &tedui::createBrush("highlight",  { 0.2, 0.2, 0.8 });
	tedui::brushes::pfocus     = &tedui::createBrush("focus",      { 0.8, 0.8, 0.8 });

	back_rect = ted::graphics::createRect(0.0, tedui::brushes::pback, { 0, 0, ted::winSize.width, ted::winSize.height });
}

TEDUI_HANDLER void tedui_shutdown()
{
	back_rect.release();
	for (auto& e : tedui::brushMap)
		e.second.release();
}

TEDUI_HANDLER void tedui_mmove(uint16_t x, uint16_t y)
{
	// TODO: start the panel system
}
