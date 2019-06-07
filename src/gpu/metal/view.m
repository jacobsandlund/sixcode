@import AppKit
@import MetalKit

#import "gpu/view.h"

@interface ViewDelegate : NSObject<MTKViewDelegate>

- (instancetype)initWithView:(GpuView *)view size:(float2)size config:(GpuViewConfig *)config;

@end

@interface ViewDelegate {
    GpuView *_view;
    GpuViewFn _size_changed;
    GpuViewFn _draw_in_view;
}

- (instancetype)initWithView:(GpuView *)view size:(float2)size config:(GpuViewConfig *)config {
    self = [super init];
    if (self) {
        _view = view;
        _size_changed = config->size_changed;
        _draw_in_view = config->draw_in_view;

        MTKView *mtk_view = (__bridge MTKView *)view->view_impl;
        mtk_view.colorPixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
        mtk_view.preferredFramesPerSecond = config->preferred_frames_per_second;

        [self mtkView:mtk_view drawableSizeWillChange:mtk_view.drawableSize];
    }

    return self;
}

- (void)mtkView:(MTKView *)mtk_view drawableSizeWillChange:(CGSize)size {
    (void)mtk_view;
    _view->viewport_size = (float2) {
        (float)size.width,
        (float)size.height,
    };

    _size_changed(_view);
}

- (void)drawInMTKView:(MTKView *)mtk_view {
    _draw_in_view(_view);
}

@end

void gpu_view_init(GpuView *view, GpuDevice *device, float2 viewport_size, GpuViewConfig *config)
{
    MTLDevice *mtl_device = (__bridge MTLDevice *)device->device_impl;

    NSRect frame = NSMakeRect(0.0, 0.0, size.x, size.y);
    MTKView *mtk_view = [[MTKView alloc] initWithFrame: frame
            device: mtl_device];

    ViewDelegate *delegate = [[ViewDelegate alloc]
            initWithView:view size:viewport_size config:config];
    [delegate retain];
    mtk_view.delegate = delegate;

    view->view_impl = (void *) CFBridgingRetain(mtk_view);
}

void gpu_view_destroy(GpuView *view)
{
    MTKView *mtk_view = (__bridge MTKView *)view->view_impl;
    [mtk_view.delegate release];
    CFRelease(view->view_impl);
}
