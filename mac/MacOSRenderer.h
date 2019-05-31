@import MetalKit;

#import "event-queue.h"
#import "renderer.h"
#import "world.h"

@interface MacOSRenderer : NSObject<MTKViewDelegate>

- (instancetype)initWithRenderer:(Renderer *)r mtkView:(MTKView *)mtkView eventQueue:(EventQueue *)eq world:(World *)w;
- (void)renderWithWorld:(World *)w;

@end

void renderer_initialize(Renderer *r, MTKView *mtkView, EventQueue *eq, World *w);
void renderer_terminate(Renderer *r);
