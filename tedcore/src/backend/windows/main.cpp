#include <Windows.h>

#include <tedcore/backend/windows/ted.hpp>
#include <tedcore/tedcore.hpp>

LRESULT on_paint(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		return on_paint(wparam, lparam);

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
