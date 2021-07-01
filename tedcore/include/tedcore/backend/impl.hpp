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

namespace ted
{
	class TEDCORE_API Line;
	class TEDCORE_API Poly;
	class TEDCORE_API Rect;
	class TEDCORE_API Bitmap;

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
				Line,
				Rect,
				Poly,
				Bitmap
			>;

			// returns NULL on error
			template<typename T>
			TEDCORE_API const T* addElem(const T& e, float depth);

			template<typename T>
			TEDCORE_API void delElem(const T* e);
		}

		template<class T>
		class RBHead
		{
			template<class U>
			friend const U* render_buf::addElem(const U& e, float depth);

			template<class U>
			friend void render_buf::delElem(const U* e);

			render_buf::type_id id;
		public:
			RBHead() : id(render_buf::type_id_map::get_id<T>()) {}
		};
	}
}

#endif