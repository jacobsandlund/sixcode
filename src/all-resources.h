#ifndef AllResourceDescriptors_h
#define AllResourceDescriptors_h

#include "spacetime.h"
#include "resource-manager.h"
#include "layout.h"

extern const char *AllResourceDescriptorsFillMeshIds[LayoutTypeNumTypes];

void all_resource_descriptors_load_descriptors(ResourceManager *rm);

#endif // AllResourceDescriptors_h
