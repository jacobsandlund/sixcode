#ifndef GpuBufferManager_h
#define GpuBufferManager_h

#include "spacetime.h"
#include "layout.h"
#include "gpu-device.h"
#include "mesh.h"

#define GpuBufferManagerMeshMaxSize 8
#define GpuBufferManagerNumMeshes 4	// 8, 4, 2, 1

typedef struct {
	GpuBuffer vertices;
	GpuBuffer indices;
} GpuBufferManagerBuffers;

typedef struct {
	GpuBufferManagerBuffers buffers[GpuBufferManagerNumMeshes];
	FillMesh meshes[GpuBufferManagerNumMeshes];
} GpuBufferManagerLayoutData;

typedef struct {
	GpuBufferManagerLayoutData layouts[LayoutTypeNumTypes];

	GpuBuffer *grid_styles_buffer;
	GpuBuffer *instance_buffer;
	InstanceMesh instance_mesh;
} GpuBufferManager;

void gpu_buffer_manager_init(GpuBufferManager *manager, GpuDevice *device);
void gpu_buffer_manager_destroy(GpuBufferManager *manager);

#endif // GpuBufferManager_h
