@import AppKit;
@import MetalKit;

#import "spacetime.h"
#import "gpu/device.h"
#import "gpu/view.h"
#import "gpu/renderer.h"

@interface MetalRenderer : NSObject

- (instancetype)initWithDevice:(GpuDevice *)device view:(GpuView *)view viewportSize:(float2)viewport_size;
- (void)drawInView:(GpuView *)view;

@property(nonatomic) float2 viewport_size;

@end
