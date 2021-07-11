#ifndef TEDCORE_IMPL_H
#define TEDCORE_IMPL_H

#include <vector>
#include <tedlang/tedl.h>

#ifdef _MSC_VER
//  Microsoft 
#	define EXPORT __declspec(dllexport)
#	define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
//  GCC
#	define EXPORT __attribute__((visibility("default")))
#	define IMPORT
#else
//  do nothing and hope for the best?
#	define EXPORT
#	define IMPORT
#	pragma warning Unknown dynamic link import/export semantics.
#endif


#ifndef TEDCORE_API
#	ifdef TEDCORE_SRC
#		define TEDCORE_API EXPORT
#	else
#		define TEDCORE_API IMPORT
#	endif
#endif

#if defined(TEDC_DIRECT2D)
#ifndef _MSC_VER
#error Direct2d is only supported on windows
#endif
#include <d2d1.h>
#elif defined(TEDC_OPENGL)
#error The OpenGL backend for tedcore has not been implemented
#else
#error Unable to determine a graphics backend
#endif

namespace ted
{
	class TEDCORE_API Empty;
	class TEDCORE_API Line;
	class TEDCORE_API Rect;

	namespace events
	{
		using key = uint8_t;

		TEDCORE_API extern std::vector<void(*)()> startupHandlers;
		TEDCORE_API extern std::vector<void(*)()> shutdownHandlers;
		TEDCORE_API extern std::vector<void(*)()> minimizeHandlers;
		TEDCORE_API extern std::vector<void(*)()> maximizeHandlers;
		TEDCORE_API extern std::vector<void(*)()> winSizeHandlers;
		TEDCORE_API extern std::vector<void(*)()> winMoveHandlers;
		TEDCORE_API extern std::vector<void(*)(key)> keyDownHandlers;
		TEDCORE_API extern std::vector<void(*)(key)> keyUpHandlers;
		TEDCORE_API extern std::vector<void(*)()> mouseLDownHandlers;
		TEDCORE_API extern std::vector<void(*)()> mouseLUpHandlers;
		TEDCORE_API extern std::vector<void(*)()> mouseMDownHandlers;
		TEDCORE_API extern std::vector<void(*)()> mouseMUpHandlers;
		TEDCORE_API extern std::vector<void(*)()> mouseRDownHandlers;
		TEDCORE_API extern std::vector<void(*)()> mouseRUpHandlers;
		TEDCORE_API extern std::vector<void(*)()> mouseMoveHandlers;
	}

	namespace impl
	{
		// Initializes tedlang, and starts any extensions
		TEDCORE_API void init();
		void buildApi(te_scope_st* pscope);

		// singleton class
		namespace render_buf
		{
			using type_id = uint32_t;
			using rb_handle = uint64_t;
			constexpr uint32_t invalid_id = -1;

			template<class... _tys>
			struct _type_id_map;

			template<>
			struct _type_id_map<>
			{
				template<class T>
				static constexpr type_id _get_id(type_id id) { return 0; }

				template<class T>
				static constexpr type_id get_id() { return -1; }

				inline static constexpr size_t type_size(type_id id) { return 0; }
			};

			template<class _ty, class... _tys>
			struct _type_id_map<_ty, _tys...>
			{
				template<class T>
				static constexpr type_id _get_id(type_id id)
				{
					if (std::is_same<_ty, T>())
						return id;
					return _type_id_map<_tys...>::template _get_id<T>(id + 1);
				}

				template<class T>
				static constexpr type_id get_id() { return _type_id_map<_ty, _tys...>::template _get_id<T>(type_id()); }

				// returns the size of the type which cooresponds to the type id in bytes
				inline static constexpr size_t type_size(type_id id)
				{
					if (id == 0) return sizeof(_ty);
					return _type_id_map<_tys...>::type_size(id - 1);
				}
			};
			
			using type_id_map = _type_id_map
			<
				Empty,
				Line,
				Rect
			>;

			class RBEHead
			{
				inline static rb_handle _rbe_id = 0;

			public:
				float depth;
				render_buf::type_id id;
				uint32_t elemsz;
				rb_handle handle;

				template<class T>
				RBEHead(T*, float depth) :
					depth(depth),
					id(render_buf::type_id_map::get_id<T>()),
					elemsz(sizeof(T)),
					handle(_rbe_id++)
				{}
			};

			TEDCORE_API void* rb_malloc(float depth, size_t sz);
			TEDCORE_API void rb_free(rb_handle handle);

			TEDCORE_API void* rb_ptr();
			TEDCORE_API void rb_pack();

#ifdef TEDC_DIRECT2D
			struct LineDef
			{
				RBEHead head;
				D2D1_POINT_2F p1;
				D2D1_POINT_2F p2;
				ID2D1Brush* brush;
				FLOAT width;
				ID2D1StrokeStyle* stroke;
			};

			struct RectDef
			{
				RBEHead head;
				D2D1_RECT_F rect;
				ID2D1Brush* brush;
			};
#endif
		}
	}
}

#endif