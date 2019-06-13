#ifndef TestAllocator_h
#define TestAllocator_h

#include <stdint.h>
#include <stdbool.h>

#define TestAllocatorMemorySize 5000000
#define TestAllocatorFreedSentinel 0xA9

void test_allocator_reset(void);
void *tmalloc(size_t size);
void tfree(void *mem);
bool test_allocator_was_freed(void *mem);

/////////////////////////////////////////////
//
// Implementation
//

static uint8_t tmem[TestAllocatorMemorySize];
static uint8_t *tmem_p = tmem;

void test_allocator_reset(void)
{
    tmem_p = tmem;
}

void *tmalloc(size_t size)
{
	void *p = tmem_p;
	tmem_p += size;
	return p;
}

void tfree(void *mem)
{
    // Memory gets deallocated when test_allocator_reset
    // is called (at end of test)
    uint8_t *mem8 = (uint8_t *) mem;
    *mem8 = TestAllocatorFreedSentinel;
}

bool test_allocator_was_freed(void *mem)
{
    uint8_t *mem8 = (uint8_t *) mem;
    return *mem8 == TestAllocatorFreedSentinel;
}

#endif // TestAllocator_h
