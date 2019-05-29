#include "test.h"
#include "resource/pointer-allocator.c"

Test(resource_pointer_allocator_alloc)
{
	ResourcePointerAllocator *pa = tmalloc(sizeof *pa);
	resource_pointer_allocator_init(pa, 16);

	_d(pa->capacity);
	//=> 16
	_d(pa->allocated);
	//=> 0
	_d(pa->memory != NULL);
	//=> 1
	_d(resource_pointer_allocator_top(pa) == pa->memory);
	//=> 1

	void *old_p = pa->memory;
	void *p = resource_pointer_allocator_alloc(pa, 10);
	void *top = resource_pointer_allocator_top(pa);

	_d(p == pa->memory);
	//=> 1
	_d(p != top);
	//=> 1
	_d(pa->allocated);
	//=> 10

	old_p = p;
	p = resource_pointer_allocator_alloc(pa, 4);

	_d(p != old_p);
	//=> 1
	_d(pa->allocated);
	//=> 14

	// Allocate over capacity

	_d(resource_pointer_allocator_alloc(pa, 6));
	//=> 0

	_Log();
	//=> Resource pointer_allocator attempt to allocate 6 bytes with only 2 bytes remaining

	// Reset top

	resource_pointer_allocator_reset_top(pa, top);

	_d(resource_pointer_allocator_top(pa) == top);
	//=> 1

	resource_pointer_allocator_destroy(pa);
}
