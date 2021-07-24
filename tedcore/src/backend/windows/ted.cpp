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

		void repaint()
		{
			InvalidateRect(hwnd, NULL, FALSE);
		}

		void getWinRect(ted::Rect& r)
		{
			RECT rc;
			GetWindowRect(hwnd, &rc);
			r.left = rc.left;
			r.top = rc.top;
			r.right = rc.right;
			r.bottom = rc.bottom;
		}
	}
}
