#ifndef TEDCORE_H
#define TEDCORE_H

#include <tuple>
#include <string>
#include <functional>
#include <tedcore/backend/impl.hpp>

namespace ted
{
	TEDCORE_API extern void(*messageBox)(const std::string& msg);

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
		struct Color
		{
			uint8_t r;
			uint8_t g;
			uint8_t b;
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

		// TODO: make the brush system

		//
		// Renderable types
		//

		struct TEDCORE_API Renderable
		{
			impl::render_buf::rb_handle handle;
			impl::render_buf::RBEHead* pdef;

			void findDef();
		};

		class TEDCORE_API Line :
			public Renderable
		{
			friend TEDCORE_API Line makeLine(float depth, Point p1, Point p2, float width);
			Line(impl::render_buf::LineDef* pdef);

		public:
			// TODO: create helper methods for transforming the line
		};

		class TEDCORE_API Rect :
			public Renderable
		{
			friend TEDCORE_API Rect makeRect(float depth, float left, float right, float top, float bottom);
			Rect(impl::render_buf::RectDef* pdef);

		public:
			// TODO: create helper methods for transforming the rect
		};

		TEDCORE_API Line makeLine(float depth, Point p1, Point p2, float width);
		TEDCORE_API Rect makeRect(float depth, float left, float right, float top, float bottom);
	}
}

#endif
