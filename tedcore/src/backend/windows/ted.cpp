#include <windows.h>
#include <d2d1.h>

#include <tedcore/backend/windows/ted.hpp>

HWND hwnd = NULL;
LPCWSTR cls_name = L"TED";
LPCWSTR win_name = L"";

ID2D1Factory* pDFactory = NULL;
ID2D1HwndRenderTarget* pDRendTarget = NULL;
