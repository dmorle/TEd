#ifndef TEDCORE_H
#define TEDCORE_H

#include <tuple>
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
		TEDCORE_API void regHandler(T handler);

		struct point
		{
			size_t x;
			size_t y;
		};

		TEDCORE_API point getMousePos();
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
		// Renderable types
		//

		class TEDCORE_API Line
		{
			impl::RBHead<Rect> head{};

		public:
		};

		class TEDCORE_API Poly
		{
			impl::RBHead<Rect> head{};

		public:
		};

		class TEDCORE_API Rect
		{
			impl::RBHead<Rect> head{};

		public:
		};

		class TEDCORE_API Bitmap
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

		template<typename T>
		constexpr const T*(*addElem)(const T&, float) = impl::render_buf::addElem<T>;
		template<typename T>
		constexpr void(*delElem)(const T*) = impl::render_buf::delElem<T>;
	}

	void messageBox(const std::string& msg);
}

#endif
