#ifndef WINDOWS_TED_H
#define WINDOWS_TED_H

#include <tedcore/tedcore.hpp>

#include <windows.h>
#include <string>

#if defined(TEDC_DIRECT2D)
#include <d2d1.h>
#elif defined(TEDC_OPENGL)
#endif

namespace ted
{
	namespace windows
	{
		extern HWND hwnd;
		extern LPCSTR cls_name;
		extern LPCSTR win_name;
		extern ID2D1Factory* pFactory;

		void messageBox(const std::string& msg);
		void repaint();
		void getWinRect(ted::Rect& r);
	}
}

#endif
