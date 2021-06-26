#ifndef TEDCORE_IMPL_H
#define TEDCORE_IMPL_H

#include <vector>
#include <tedcore/tedcore.hpp>

namespace ted
{
	namespace impl
	{
		inline std::vector<std::function<void>> startupHandlers;
		inline std::vector<std::function<void>> shutdownHandlers;
		inline std::vector<std::function<void>> minimizeHandlers;
		inline std::vector<std::function<void>> maximizeHandlers;
		inline std::vector<std::function<void>> wSizeHandlers;
		inline std::vector<std::function<void>> wMoveHandlers;
		inline std::vector<std::function<void>> keyDownHandlers;
		inline std::vector<std::function<void>> keyUpHandlers;
		inline std::vector<std::function<void>> mouseDownHandlers;
		inline std::vector<std::function<void>> mouseUpHandlers;
		inline std::vector<std::function<void>> mouseMoveHandlers;

		// Initializes tedlang, and starts any extensions
		void init();

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
			const T* addElem(const T& e, float depth);

			template<typename T>
			void delElem(const T* e);
		}

		template<class T>
		class RBHead
		{
			template<class U>
			friend const U* addElem(const U& e, float depth);

			template<class U>
			friend void delElem(const T* e);

			render_buf::type_id id;
		public:
			RBHead() : id(type_id_map::get_id<T>()) {}
		};
	}
}

#endif