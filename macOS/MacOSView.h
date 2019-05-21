@import MetalKit;

#import "spacetime.h"
#import "view.h"

@interface MacOSView : MTKView

@property (readonly) BOOL dragging;

- (instancetype) initWithView:(View *)vw device:(id<MTLDevice>)device;

@end

void view_initialize(View *vw, float2 viewport_size);
void view_terminate(View *vw);
