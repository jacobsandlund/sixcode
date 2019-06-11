#import "gpu/metal/view.h"
#import "log/manager.h"

@implementation ViewDelegate {
    GpuView *_view;
    GpuViewFn _size_changed;
    GpuViewFn _draw_in_view;
}

- (instancetype)initWithView:(GpuView *)view mtkView:(MTKView *)mtk_view config:(GpuViewConfig *)config {
    self = [super init];
    if (self) {
        _view = view;
        _size_changed = config->size_changed;
        _draw_in_view = config->draw_in_view;
        _mtk_view = mtk_view;

        [self mtkView:mtk_view drawableSizeWillChange:mtk_view.drawableSize];
    }

    return self;
}

- (void)mtkView:(MTKView *)mtk_view drawableSizeWillChange:(CGSize)size {
    (void)mtk_view;
    LogDebug(&gLogManager.logs.os, "drawableSizeWillChange %g, %g", (float) size.width, (float) size.height);
    _view->viewport_size = (float2) {
        (float)size.width,
        (float)size.height,
    };

    _size_changed(_view);
}

- (void)drawInMTKView:(MTKView *)mtk_view {
    (void)mtk_view;
    _draw_in_view(_view);
}

@end

void gpu_view_init(GpuView *view, GpuDevice *device, float2 viewport_size, GpuViewConfig *config)
{
    id<MTLDevice> mtl_device = (__bridge id<MTLDevice>)device->device_impl;

    (void) viewport_size;
    NSRect frame = [NSScreen.mainScreen frame];
    MTKView *mtk_view = [[MTKView alloc] initWithFrame: frame
            device: mtl_device];
    mtk_view.colorPixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
    mtk_view.preferredFramesPerSecond = config->preferred_frames_per_second;

    ViewDelegate *delegate = [[ViewDelegate alloc]
            initWithView:view mtkView:mtk_view config:config];
    mtk_view.delegate = delegate;

    view->view_impl = (void *) CFBridgingRetain(delegate);
}

void gpu_view_destroy(GpuView *view)
{
    CFRelease(view->view_impl);
}
