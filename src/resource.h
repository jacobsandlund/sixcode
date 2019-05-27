#ifndef Resource_h
#define Resource_h

#include "spacetime.h"

#define ResourceMaxNumDependencies 6

typedef enum {
	GpuResourceTypeBuffer,
	ResourceTypeNumTypes,
} ResourceType;

typedef struct {
	void *pointer;
	sid id;
	sid dependencies[ResourceMaxNumDependencies];
} Resource;

typedef struct {
	Resource resource;
	ResourceType type;
	uintptr_t options;
} ResourceDescriptor;

typedef void (*ResourceInitFn)(Resource *resource, uintptr_t options);
typedef void (*ResourceDestroyFn)(Resource *resource);

typedef struct {
	ResourceType type;
	ResourceInitFn init;
	ResourceDestroyFn destroy;
} ResourceLoader;

extern const ResourceInitFn resource_default_init;
extern const ResourceDestroyFn resource_default_destroy;

#endif // Resource_h
