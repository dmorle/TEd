#ifndef TEDLANG_STRING_H
#define TEDLANG_STRING_H

#include <stdint.h>

extern size_t __cdecl te_strlen(char* str);

// Does not copy null termination character
extern size_t __cdecl te_strncpy(char* dst, char* src, size_t sz);

extern size_t __cdecl te_memcpy(void* dst, void* src, size_t sz);

extern char* __cdecl te_itoa(char* dst, int64_t val, int64_t radix);

#endif
