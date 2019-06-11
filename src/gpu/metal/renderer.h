@import AppKit;
@import MetalKit;

#import "spacetime.h"
#import "gpu/device.h"
#import "gpu/view.h"
#import "gpu/renderer.h"

@interface MetalRenderer : NSObject

- (instancetype)initWithDevice:(GpuDevice *)device view:(GpuView *)view;
- (void)renderWithView:(GpuView *)view;

@end
