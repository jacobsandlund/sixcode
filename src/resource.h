#ifndef Resource_h
#define Resource_h

#include "spacetime.h"

#define ResourceMaxNumDependencies 2

typedef enum {
	ResourceTypeList,
	GpuResourceTypeBuffer,
	ResourceTypeNumTypes,
} ResourceType;

typedef struct {
	sid id;
	void *pointer;
	void *dependencies[ResourceMaxNumDependencies];
} Resource;

typedef struct {
	sid id;
	sid dependencies[ResourceMaxNumDependencies];
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

extern const ResourceInitFn ResourceDefaultInitFn;
extern const ResourceDestroyFn ResourceDefaultDestroyFn;

void resource_register_loaders(void);

#endif // Resource_h
