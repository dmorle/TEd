#ifndef WINDOWS_TED_H
#define WINDOWS_TED_H

#include <windows.h>

#include <string>

namespace ted {
	namespace windows
	{
		extern HWND hwnd;
		extern LPCWSTR cls_name;
		extern LPCWSTR win_name;

		void messageBox(const std::string& msg);
	}
}

#endif
