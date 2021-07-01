#include <tedcore/backend/windows/ted.hpp>
#include <tedcore/tedcore.hpp>

using namespace ted;
using namespace windows;
using namespace impl;
using namespace events;

LRESULT on_ldown(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

LRESULT on_lup(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

LRESULT on_mdown(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

LRESULT on_mup(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

LRESULT on_rdown(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

LRESULT on_rup(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

LRESULT on_mmove(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

LRESULT on_mwheel(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

LRESULT on_keydown(WPARAM wparam, LPARAM lparam)
{
	for (auto& e : keyDownHandlers)
		e(wparam);
	return 0;
}

LRESULT on_keyup(WPARAM wparam, LPARAM lparam)
{
	for (auto& e : keyUpHandlers)
		e(wparam);
	return 0;
}

LRESULT on_size(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

LRESULT on_paint(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

LRESULT on_create(WPARAM wparam, LPARAM lparam)
{
	ted::messageBox = ted::windows::messageBox;

	init();
	for (auto& e : startupHandlers)
		e();
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_LBUTTONDOWN:
		return on_ldown(wparam, lparam);

	case WM_LBUTTONUP:
		return on_lup(wparam, lparam);

	case WM_MBUTTONDOWN:
		return on_mdown(wparam, lparam);

	case WM_MBUTTONUP:
		return on_mup(wparam, lparam);

	case WM_RBUTTONDOWN:
		return on_rdown(wparam, lparam);

	case WM_RBUTTONUP:
		return on_rup(wparam, lparam);

	case WM_MOUSEMOVE:
		return on_mmove(wparam, lparam);

	case WM_MOUSEWHEEL:
		return on_mwheel(wparam, lparam);

	case WM_KEYDOWN:
		return on_keydown(wparam, lparam);

	case WM_KEYUP:
		return on_keyup(wparam, lparam);

	case WM_SIZE:
		return on_size(wparam, lparam);

	case WM_PAINT:
		return on_paint(wparam, lparam);

	case WM_CREATE:
		return on_create(wparam, lparam);

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	WNDCLASS wc = { NULL };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = "ted";

	RegisterClass(&wc);

	hwnd = CreateWindowEx(
		0,
		"ted",
		"ted",
		0,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hwnd)
		return 0;

	ShowWindow(hwnd, nCmdShow);

	MSG msg = { NULL };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
