#include <inttypes.h>
#include <stdlib.h>
#include "resource/pointer-allocator.h"

void resource_pointer_allocator_init(ResourcePointerAllocator *pa, u64 capacity)
{
	pa->memory = malloc(capacity);
	pa->capacity = capacity;
	pa->allocated = 0;
}

void resource_pointer_allocator_destroy(ResourcePointerAllocator *pa)
{
	free(pa->memory);
}

void *resource_pointer_allocator_alloc(ResourcePointerAllocator *pa, u64 size)
{
	void *pointer = &pa->memory[pa->allocated];
	pa->allocated += size;
	if (pa->allocated > pa->capacity) {
		Log("Resource pointer_allocator attempt to allocate %"
				PRIu64 " bytes with only %" PRIu64 " bytes remaining",
				size,
				pa->capacity - pa->allocated + size);
		return NULL;
	}

	DLog("Resource mallocating %" PRIu64 " bytes (%"
			PRIu64 " bytes total)", size, pa->allocated);
	return pointer;
}

void *resource_pointer_allocator_top(ResourcePointerAllocator *pa)
{
	return &pa->memory[pa->allocated];
}

void resource_pointer_allocator_reset_top(ResourcePointerAllocator *pa, void *top)
{
	i64 set_allocated = ((u8 *) top) - pa->memory;
	if (set_allocated < 0 || (u64) set_allocated > pa->capacity) {
		Log("Resource pointer_allocator attempt to reset top to invalid address (%"
				PRIi64 " bytes away from start)",
				set_allocated);
	} else {
		pa->allocated = set_allocated;
	}
}
