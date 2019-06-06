#ifndef Resource_h
#define Resource_h

#include "spacetime.h"

#define ResourceMaxNumDependencies 2

typedef struct {
    sid id;
    void *pointer;
    void *dependencies[ResourceMaxNumDependencies];
} Resource;

typedef struct {
    sid id;
    sid dependencies[ResourceMaxNumDependencies];
    sid type;
    uintptr_t options;
} ResourceDescriptor;

typedef void (*ResourceInitFn)(Resource *resource, uintptr_t options);
typedef void (*ResourceDestroyFn)(Resource *resource);

typedef struct {
    ResourceInitFn init;
    ResourceDestroyFn destroy;
} ResourceLoader;

extern const ResourceInitFn ResourceDefaultInitFn;
extern const ResourceDestroyFn ResourceDefaultDestroyFn;

#endif // Resource_h
