#ifndef GpuBufferManager_h
#define GpuBufferManager_h

#include "spacetime.h"
#include "layout.h"
#include "gpu-device.h"

#define GpuBufferManagerMeshMaxSize 8
#define GpuBufferManagerNumMeshes 4	// 8, 4, 2, 1

typedef struct {
	Buffer vertices;
	Buffer indices;
} BufferManagerBuffers;

typedef struct {
	BufferManagerBuffers buffers[BufferManagerNumMeshes];
	FillMesh meshes[BufferManagerNumMeshes];
} BufferManagerLayoutData;

typedef struct {
	BufferManagerLayoutData layouts[LayoutTypeNumTypes];

	void *os_grid_styles_buffer;
	void *os_instance_buffer;
	InstanceMesh instance_mesh;
} BufferManager;

void buffer_manager_initialize(BufferManager *bm);

#endif // GpuBufferManager_h
