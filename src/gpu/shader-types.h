#ifndef _GpuShaderTypes_h
#define _GpuShaderTypes_h

// Needs "spacetime.h" in c/m files

typedef enum {
    GpuVertexIndexVertices = 0,
    GpuVertexIndexViewportSize = 1,
} GpuVertexInputIndex;

typedef struct {
    float2 position;
    float4 color;
} GpuVertex;

#endif // _GpuShaderTypes_h
