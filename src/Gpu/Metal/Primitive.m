#import "Gpu/Metal/Primitive.h"

@import MetalKit;

// Order must match GpuPrimitiveType in gpu/primitive.h
const i32 GpuMetalPrimitiveTypeLookup[] = {
    MTLPrimitiveTypeTriangle,
};
