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
	PAINTSTRUCT ps;
	BeginPaint(hwnd, &ps);
	render_buf::pRenderTarget->BeginDraw();

	uint8_t* prb = (uint8_t*)render_buf::rb_ptr();
	size_t n = render_buf::rb_count();
	for (size_t i = 0; i < n; i++)
	{
		render_buf::RectDef* rectdef = ((render_buf::RectDef*)prb);

		switch (((render_buf::RBEHead*)prb)->id)
		{
		case render_buf::type_id_map::get_id<render_buf::LineDef>() :
			render_buf::pRenderTarget->DrawLine(
				((render_buf::LineDef*)prb)->p1,
				((render_buf::LineDef*)prb)->p2,
				((render_buf::LineDef*)prb)->brush,
				((render_buf::LineDef*)prb)->width,
				((render_buf::LineDef*)prb)->stroke
			);
			break;
		case render_buf::type_id_map::get_id<render_buf::RectDef>() :
			render_buf::pRenderTarget->FillRectangle(
				((render_buf::RectDef*)prb)->rect,
				((render_buf::RectDef*)prb)->brush
			);
			break;
		}
		prb += ((render_buf::RBEHead*)prb)->elemsz;
	}

	render_buf::pRenderTarget->EndDraw();  // TODO: error checking
	EndPaint(hwnd, &ps);
	return 0;
}

LRESULT on_init()
{
#if defined(TEDC_DIRECT2D)
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &pFactory)))
		return -1;

	D2D1_RECT_L rc;
	if (!GetClientRect(hwnd, &rc))
		return -1;
	D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
	if (FAILED(pFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd, size),
		&render_buf::pRenderTarget
	)))
		return -1;
#elif defined(TEDC_OPENGL)
#endif

	ted::messageBox = ted::windows::messageBox;
	ted::repaint = ted::windows::repaint;

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

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

int WINAPI WinMain(
	_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPSTR pCmdLine,
	_In_     int nCmdShow
)
{
	WNDCLASS wc = { NULL };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = "ted";

	RegisterClass(&wc);

	hwnd = CreateWindowEx(
		0,
		cls_name,
		win_name,
		WS_POPUP,
		100,
		100,
		680,
		420,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hwnd)
		return 0;

	ShowWindow(hwnd, nCmdShow);

	if (on_init() < 0)
		return -1;

	MSG msg = { NULL };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
