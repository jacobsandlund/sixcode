#ifndef Resource_h
#define Resource_h

#include "spacetime.h"

typedef enum {
	ResourceTypeFillMesh,
	ResourceTypeInstanceMesh,
	GpuResourceTypeBuffer,
	ResourceTypeNumTypes,
} ResourceType;

typedef struct {
	char *id;
	char **dependencies;
	i64 num_dependencies;
	ResourceType type;
	uintptr_t options;
} ResourceDescriptor;

typedef struct {
	ResourceDescriptor descriptor;
	void *pointer;
} Resource;

typedef void *(*ResourceInitFn)(uintptr_t options);
typedef void (*ResourceDestroyFn)(void *pointer);

typedef struct {
	ResourceType type;
	ResourceInitFn init;
	ResourceDestroyFn destroy;
} ResourceLoader;

extern const ResourceInitFn resource_default_init;
extern const ResourceDestroyFn resource_default_destroy;

#endif // Resource_h
