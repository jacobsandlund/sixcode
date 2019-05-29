#ifndef SHADER_TYPES_H
#define SHADER_TYPES_H

typedef enum {
    VertexInputIndexVertices = 0,
    VertexInputIndexViewportSize = 1,
} VertexInputIndex;

typedef struct {
    float2 position;
    float4 color;
} Vertex;

#endif /* SHADER_TYPES_H */
