#ifndef TEDCORE_H
#define TEDCORE_H

#include <tuple>
#include <string>
#include <functional>
#include <tedcore/backend/impl.hpp>

namespace ted
{
	TEDCORE_API extern void(*messageBox)(const std::string& msg);
	TEDCORE_API extern void(*repaint)();

	struct Point
	{
		float x;
		float y;
	};

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

		class TEDCORE_API Line;
		class TEDCORE_API Rect;

		TEDCORE_API ted::graphics::Line createLine(float depth, Brush* brush, Point p1, Point p2, float width);
		TEDCORE_API ted::graphics::Rect createRect(float depth, Brush* brush, float left, float top, float right, float bottom);

		struct Color
		{
			float r;
			float g;
			float b;
		};

		class TEDCORE_API Typeface
		{
			friend class Bitmap;

		public:
			Typeface(const std::string& fname);
			Typeface(const std::string& fname, const Color& c);
		};

		//
		// Brush system
		//

		class TEDCORE_API Brush
		{
			friend TEDCORE_API ted::graphics::Line ted::graphics::createLine(float, Brush*, Point, Point, float);
			friend TEDCORE_API ted::graphics::Rect ted::graphics::createRect(float, Brush*, float, float, float, float);

			impl::render_buf::BrushDef* def;

		public:
			Brush(const Color& c);
			void release();
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

		class TEDCORE_API Line :
			public Renderable
		{
			friend ted::graphics::Line ted::graphics::createLine(float, Brush*, Point, Point, float);
			Line(impl::render_buf::LineDef* pdef);

		public:
			void release();

			// TODO: create helper methods for transforming the line
		};

		class TEDCORE_API Rect :
			public Renderable
		{
			friend ted::graphics::Rect ted::graphics::createRect(float, Brush*, float, float, float, float);
			Rect(impl::render_buf::RectDef* pdef);

		public:
			void release();

			// TODO: create helper methods for transforming the rect
		};
	}
}

#endif
