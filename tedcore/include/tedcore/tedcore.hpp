#ifndef TEDCORE_H
#define TEDCORE_H

#include <tuple>
#include <string>
#include <functional>
#include <tedcore/backend/impl.hpp>

namespace ted
{
	struct Point
	{
		float x;
		float y;
	};

	struct Rect
	{
		float left;
		float top;
		float right;
		float bottom;
	};

	struct Size
	{
		float width;
		float height;
	};

	TEDCORE_API extern void(*messageBox)(const std::string& msg);
	TEDCORE_API extern void(*repaint)();
	TEDCORE_API extern void(*getWinRect)(Rect&);
	TEDCORE_API extern Size winSize;

	namespace events
	{
		enum class event
		{
			STARTUP,
			SHUTDOWN,
			MINIMIZE,
			MAXIMIZE,
			WINDOW_SIZE,
			WINDOW_MOVE,
			KEY_DOWN,
			KEY_UP,
			MOUSE_DOWN,
			MOUSE_UP,
			MOUSE_MOVE
		};

		template<event E, class T>
		TEDCORE_API void regHandler(T handler);

		TEDCORE_API Point getMousePos();
		TEDCORE_API bool getKeyState();
		TEDCORE_API bool getMouseState();
	}
	
	namespace graphics
	{
		// forward declarations

		class TEDCORE_API Brush;

		class TEDCORE_API GLine;
		class TEDCORE_API GRect;

		TEDCORE_API ted::graphics::GLine createLine(float depth, Brush* brush, Point p1, Point p2, float width);
		TEDCORE_API ted::graphics::GRect createRect(float depth, Brush* brush, Rect rc);

		struct Color
		{
			float r;
			float g;
			float b;
		};

		class TEDCORE_API Typeface
		{
			friend class GBitmap;

		public:
			Typeface(const std::string& fname);
			Typeface(const std::string& fname, const Color& c);
		};

		//
		// Brush system
		//

		class TEDCORE_API Brush
		{
		public:
			Brush(const Color& c);
			void release();

			void setColor(const Color& c);

			impl::render_buf::BrushDef* def;
		};

		//
		// Renderable types
		//

		class TEDCORE_API Renderable
		{
		protected:
			impl::render_buf::rb_handle handle;
			impl::render_buf::RBEHead* pdef;

		public:
			void findDef();
			void release();
		};

		class TEDCORE_API GLine :
			public Renderable
		{
			friend GLine createLine(float, Brush*, Point, Point, float);
			GLine(impl::render_buf::LineDef* pdef);

			inline impl::render_buf::LineDef* def();

		public:
			void release();

			void setBrush(Brush* pBrush);
		};

		class TEDCORE_API GRect :
			public Renderable
		{
			friend GRect createRect(float depth, Brush* brush, Rect rc);
			GRect(impl::render_buf::RectDef* pdef);

			inline impl::render_buf::RectDef* def();

		public:
			void release();

			void setBrush(Brush* pBrush);
		};
	}
}

#endif
