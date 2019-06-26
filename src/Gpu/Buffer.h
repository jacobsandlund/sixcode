#ifndef _GpuBuffer_h
#define _GpuBuffer_h

#include "Gpu/Device.h"
#include "Spacetime.h"

typedef enum {
    GpuBufferStorageModeShared,
} GpuBufferStorageMode;

typedef struct GpuBuffer GpuBuffer;

GpuBuffer *GpuBufferCreateWithLength(GpuDevice *device,
                                     i64 length,
                                     GpuBufferStorageMode storage_mode);
void GpuBufferDestroy(GpuBuffer *buffer);
void *GpuBufferContents(GpuBuffer *buffer);

#endif  // _GpuBuffer_h
