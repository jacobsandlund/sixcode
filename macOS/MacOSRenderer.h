@import MetalKit;

#import "MacOSView.h"
#import "renderer.h"
#import "view.h"
#import "world.h"

@interface MacOSRenderer : NSObject<MTKViewDelegate>

- (instancetype)initWithRenderer:(Renderer *)r view:(View *)vw world:(World *)w;
- (void)renderWithView:(View *)vw world:(World *)world;

@end

void renderer_initialize(Renderer *r, View *vw, World *w);
void renderer_terminate(Renderer *r);
