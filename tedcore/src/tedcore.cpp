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
	TEDCORE_API void(*repaint)() = NULL;
	TEDCORE_API void(*getWinRect)(Rect&) = NULL;
	TEDCORE_API Size winSize = { 0.0, 0.0 };

	namespace graphics
	{
		using namespace render_buf;

		Brush::Brush(const Color& c) :
			def(NULL)
		{
			pRenderTarget->CreateSolidColorBrush(
				D2D1_COLOR_F{ c.r, c.g, c.b, 1.0f },
				(ID2D1SolidColorBrush**)&this->def
			);
		}

		void Brush::release()
		{
			if (this->def)
				this->def->Release();
		}

		void Brush::setColor(const Color& c)
		{
			this->def->SetColor(D2D1_COLOR_F{ c.r, c.g, c.b, 1.0f });
		}

		void Renderable::findDef()
		{
			this->pdef = (RBEHead*)rb_ptr();
			while (this->pdef->handle != this->handle)
				pdef = (RBEHead*)(((uint8_t*)this->pdef) + this->pdef->elemsz);
		}

		GLine::GLine(LineDef* pdef)
		{
			this->pdef = (RBEHead*)pdef;
			this->handle = pdef->head.handle;
		}

		impl::render_buf::LineDef* GLine::def()
		{
			return (impl::render_buf::LineDef*)this->pdef;
		}

		void GLine::release()
		{
			rb_free(this->handle);
		}

		void GLine::setBrush(Brush* pBrush)
		{
			this->findDef();
			this->def()->brush = pBrush->def;
		}

		GRect::GRect(RectDef* pdef)
		{
			this->pdef = (RBEHead*)pdef;
			this->handle = pdef->head.handle;
		}

		impl::render_buf::RectDef* GRect::def()
		{
			return (impl::render_buf::RectDef*)this->pdef;
		}

		void GRect::release()
		{
			rb_free(this->handle);
		}

		void GRect::setBrush(Brush* pBrush)
		{
			this->findDef();
			this->def()->brush = pBrush->def;
		}

#ifdef TEDC_DIRECT2D
		TEDCORE_API GLine createLine(float depth, Brush* brush, Point p1, Point p2, float width)
		{
			LineDef* pldef = (LineDef*)rb_malloc(depth, sizeof(LineDef));
			pldef->head = RBEHead(pldef, depth);
			pldef->p1 = { p1.x, p1.y };
			pldef->p2 = { p2.x, p2.y };
			pldef->brush = brush->def;  // tmp
			pldef->width = width;
			pldef->stroke = NULL;
			repaint();

			return pldef;
		}

		TEDCORE_API GRect createRect(float depth, Brush* brush, Rect rc)
		{
			RectDef* prdef = (RectDef*)rb_malloc(depth, sizeof(RectDef));
			prdef->head = RBEHead(prdef, depth);
			prdef->rect = { rc.left, rc.top, rc.right, rc.bottom };
			prdef->brush = brush->def;  // tmp
			repaint();
			
			return prdef;
		}
#endif
	}
}
