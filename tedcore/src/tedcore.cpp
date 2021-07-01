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

	/*template<>
	void regHandler<event::STARTUP>(std::function<void> handler)
	{
		startupHandlers.push_back(handler);
	}

	template<>
	void regHandler<event::STARTUP>(te_obj_st* pobj)
	{
		te_incref(pobj);
		
	}

	template<>
	void regHandler<event::SHUTDOWN>(std::function<void> handler)
	{

	}*/

	TEDCORE_API void(*messageBox)(const std::string& msg) = NULL;
}
