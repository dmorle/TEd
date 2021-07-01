#include <tedcore/backend/windows/ted.hpp>

namespace ted
{
	namespace windows
	{
		HWND hwnd = NULL;
		LPCWSTR cls_name = L"TEd";
		LPCWSTR win_name = L"";

		void messageBox(const std::string& msg)
		{
			MessageBox(hwnd, msg.c_str(), "", MB_OK);
		}
	}
}
