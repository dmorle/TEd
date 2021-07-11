#include <tedcore/backend/windows/ted.hpp>

namespace ted
{
	namespace windows
	{
		HWND hwnd = NULL;
		LPCSTR cls_name = "TEd";
		LPCSTR win_name = "";
		ID2D1Factory* pFactory = NULL;

		void messageBox(const std::string& msg)
		{
			MessageBox(hwnd, msg.c_str(), "", MB_OK);
		}
	}
}
