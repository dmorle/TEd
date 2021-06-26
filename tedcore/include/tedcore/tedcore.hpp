#ifndef TEDCORE_H
#define TEDCORE_H

#include <string>
#include <functional>
#include <tedcore/backend/impl.hpp>

namespace ted
{
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
		void regHandler(T handler);
	}
	
	namespace graphics
	{
		struct Color
		{
			uint8_t r;
			uint8_t g;
			uint8_t b;
		};

		class Typeface
		{
			friend class Bitmap;

		public:
			Typeface(const std::string& fname);
			Typeface(const std::string& fname, const Color& c);
		};

		//
		// Renderable types
		//

		class Line
		{
			impl::RBHead<Rect> head{};

		public:
		};

		class Poly
		{
			impl::RBHead<Rect> head{};

		public:
		};

		class Rect
		{
			impl::RBHead<Rect> head{};

		public:
		};

		class Bitmap
		{
			impl::RBHead<Bitmap> head{};

		public:
			Bitmap(size_t width, size_t height, const Color& c);
			Bitmap(const std::string& fname);
			Bitmap(const Typeface& tf, const std::string& text);

			Bitmap(const Bitmap& bm);
			Bitmap(Bitmap&& bm);
			Bitmap& operator=(const Bitmap& bm);
			Bitmap& operator=(Bitmap&& bm);
			~Bitmap();

			size_t getWidth() const;
			size_t getHeight() const;
		};
	}
}

#endif
