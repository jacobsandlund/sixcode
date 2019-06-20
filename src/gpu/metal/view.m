#import "gpu/metal/view.h"
#import "log/manager.h"

const i64 MetalViewColorPixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;

static void gpu_view_noop_draw_in_view(GpuView *view)
{
    (void) view;
}

static void gpu_view_noop_size_changed(GpuView *view, float2 viewport_size)
{
    (void) view;
    (void) viewport_size;
}

@implementation ViewDelegate {
    GpuViewFn _draw_in_view;
    GpuViewSizeChangedFn _size_changed;
}

- (instancetype)initWithMtkView:(MTKView *)mtk_view config:(GpuViewConfig *)config {
    (void)config;

    self = [super init];
    if (self) {
        _mtk_view = mtk_view;
        _draw_in_view = gpu_view_noop_draw_in_view;
        _size_changed = gpu_view_noop_size_changed;

        [self mtkView:mtk_view drawableSizeWillChange:mtk_view.drawableSize];
    }

    return self;
}

- (void)registerCallbacks:(GpuViewCallbacks *)callbacks {
    _draw_in_view = callbacks->draw_in_view;
    _size_changed = callbacks->size_changed;
}

- (void)mtkView:(MTKView *)mtk_view drawableSizeWillChange:(CGSize)size {
    _mtk_view = mtk_view;
    _viewport_size = (float2) {
        (float)size.width,
        (float)size.height,
    };
    LogDebug(gLogManager.logs.os, "drawableSizeWillChange %g, %g", _viewport_size.x, _viewport_size.y);

    _size_changed((__bridge GpuView *)self, _viewport_size);
}

- (void)drawInMTKView:(MTKView *)mtk_view {
    _mtk_view = mtk_view;
    _draw_in_view((__bridge GpuView *)self);
}

@end

GpuView *gpu_view_create(GpuDevice *device, OsScreenFrame frame, GpuViewConfig *config)
{
    GpuView *view;

    @autoreleasepool {

    id<MTLDevice> mtl_device = (__bridge id<MTLDevice>)device;

    NSRect ns_frame = NSMakeRect(
            frame.origin.x,
            frame.origin.y,
            frame.size.x,
            frame.size.y);

    MTKView *mtk_view = [[MTKView alloc] initWithFrame: ns_frame
            device: mtl_device];
    mtk_view.colorPixelFormat = config->color_pixel_format;
    mtk_view.preferredFramesPerSecond = config->preferred_frames_per_second;

    ViewDelegate *delegate = [[ViewDelegate alloc]
            initWithMtkView:mtk_view config:config];
    mtk_view.delegate = delegate;

    view = (__bridge_retained GpuView *)delegate;

    } // @autoreleasepool

    return view;
}

void gpu_view_destroy(GpuView *view)
{
    @autoreleasepool {
        ViewDelegate *delegate = (__bridge_transfer ViewDelegate *)view;
        delegate = nil;
    }
}

void gpu_view_register_callbacks(GpuView *view, GpuViewCallbacks *callbacks)
{
    ViewDelegate *delegate = (__bridge ViewDelegate *)view;
    [delegate registerCallbacks:callbacks];
}

float2 gpu_view_viewport_size(GpuView *view)
{
    ViewDelegate *delegate = (__bridge ViewDelegate *)view;
    return delegate.viewport_size;
}

i64 gpu_view_color_pixel_format(GpuView *view)
{
    ViewDelegate *delegate = (__bridge ViewDelegate *)view;
    return delegate.mtk_view.colorPixelFormat;
}

GpuRenderPassConfig *gpu_view_current_render_pass_config(GpuView *view)
{
    ViewDelegate *delegate = (__bridge ViewDelegate *)view;

    // TODO: check if needs to be retained
    return (__bridge GpuRenderPassConfig *)delegate.mtk_view.currentRenderPassDescriptor;
}
