#ifndef TED_H
#define TED_H

#include <windows.h>
#include <d2d1.h>

extern HWND hwnd;
extern LPCWSTR cls_name;
extern LPCWSTR win_name;

extern ID2D1Factory* pDFactory;
extern ID2D1HwndRenderTarget* pDRendTarget;

struct
{
	ID2D1SolidColorBrush* back;
	ID2D1SolidColorBrush* wall;
	ID2D1SolidColorBrush* text;
}
brushes;

#endif
