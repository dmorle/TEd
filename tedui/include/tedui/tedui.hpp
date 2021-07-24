#ifndef TEDUI_H
#define TEDUI_H

#ifdef _MSC_VER
//  Microsoft 
#	define EXPORT __declspec(dllexport)
#	define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
//  GCC
#	define EXPORT __attribute__((visibility("default")))
#	define IMPORT
#else
//  do nothing and hope for the best?
#	define EXPORT
#	define IMPORT
#	pragma warning Unknown dynamic link import/export semantics.
#endif


#ifndef TEDUI_API
#	ifdef TEDUI_SRC
#       define TEDUI_HANDLER extern "C" EXPORT
#		define TEDUI_API EXPORT
#	else
#		define TEDUI_API IMPORT
#	endif
#endif

#include <map>
#include <string>

#include <tedcore/tedcore.hpp>

namespace tedui
{
	constexpr float frame_size = 1.0f;
	constexpr float panel_space = 1.0f;

	namespace brushes
	{
		extern ted::graphics::Brush* pback;
		extern ted::graphics::Brush* pbase;
		extern ted::graphics::Brush* pselect;
		extern ted::graphics::Brush* pactive;
		extern ted::graphics::Brush* phighlight;
		extern ted::graphics::Brush* pfocus;
	}

	using BrushMap = std::map<std::string, ted::graphics::Brush>;

	const BrushMap& getBrushMap();

	ted::graphics::Brush& createBrush(const std::string& name, const ted::graphics::Color& c);
}

#endif
