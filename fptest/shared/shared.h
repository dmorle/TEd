#ifndef SHARED_H
#define SHARED_H

#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)

#ifndef API
#ifdef SRC
#define API EXPORT
#else
#define API IMPORT
#endif
#endif

typedef struct
{
	void (*m_fn)();
}
test_fn_st;

API void get_fp(test_fn_st*);

#endif
