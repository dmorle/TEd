#ifndef TEDLANG_STRING_H
#define TEDLANG_STRING_H

#include <stdint.h>

extern size_t __cdecl te_strlen(const char* str);

// Does not copy null termination character
extern uint32_t __cdecl te_strncpy(char* dst, const char* src, uint32_t sz);

#endif
