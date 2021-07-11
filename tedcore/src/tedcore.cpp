#define TEDCORE_SRC

#include <iostream>

#include <tedcore/tedcore.hpp>
#include <tedcore/backend/impl.hpp>

#include <tedlang/tedl.h>

#ifdef _MSC_VER
#include <windows.h>
#endif

namespace ted
{
	using namespace impl;

	TEDCORE_API void(*messageBox)(const std::string& msg) = NULL;

	namespace graphics
	{
		using namespace render_buf;

		void Renderable::findDef()
		{
			this->pdef = (RBEHead*)rb_ptr();
			while (this->pdef->handle != this->handle)
				pdef = (RBEHead*)(((uint8_t*)this->pdef) + this->pdef->elemsz);
		}

		Line::Line(LineDef* pdef)
		{
			this->pdef = (RBEHead*)pdef;
			this->handle = pdef->head.handle;
		}

		Rect::Rect(RectDef* pdef)
		{
			this->pdef = (RBEHead*)pdef;
			this->handle = pdef->head.handle;
		}

		TEDCORE_API Line makeLine(float depth, Point p1, Point p2, float width)
		{
			LineDef* pldef = (LineDef*)rb_malloc(depth, sizeof(LineDef));
			pldef->head = RBEHead(pldef, depth);
			pldef->p1 = { p1.x, p1.y };
			pldef->p2 = { p2.x, p2.y };
			pldef->brush = NULL;  // tmp
			pldef->width = width;
			pldef->stroke = NULL;

			return pldef;
		}

		TEDCORE_API Rect makeRect(float depth, float left, float right, float top, float bottom)
		{
			RectDef* prdef = (RectDef*)rb_malloc(depth, sizeof(RectDef));
			prdef->head = RBEHead(prdef, depth);
			prdef->rect = { left, top, right, bottom };
			prdef->brush = NULL;  // tmp
			
			return prdef;
		}
	}
}
