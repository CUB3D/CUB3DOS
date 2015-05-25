#include "Memory.h"

#include <stddef.h>

size_t mallocStartPtr = 0xffffff;

void* malloc(size_t size)
{
	size_t ret = mallocStartPtr;

	mallocStartPtr += size;

	return (void*) ret;
}