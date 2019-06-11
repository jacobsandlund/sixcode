#import "os/mac/window.h"
#import "gpu/metal/view.h"
#import "log/manager.h"

// Look at this for a lot of the boilerplate here:
//   https://hero.handmade.network/forums/code-discussion/t/1409-main_game_loop_on_os_x

@implementation MacWindow

- (void)keyDown:(NSEvent *)nsEvent { (void)nsEvent; }
- (void)keyUp:(NSEvent *)nsEvent { (void)nsEvent; }

- (BOOL)acceptsFirstResponder { return YES; }
- (BOOL)canBecomeKeyWindow { return YES; }
- (BOOL)canBecomeMainWindow { return YES; }

@end

@implementation WindowDelegate

- (instancetype)initWithMacWindow:(MacWindow *)window {
    self = [super init];
    if (self) {
        _window = window;
    }
    return self;
}

- (void)loadView {
    [super loadView];
    LogDebug(&gLogManager.logs.os, "loadView called");
}

@end

void os_window_init(OsWindow *window)
{
    MacWindow *mac_window = [[MacWindow alloc] init];
    mac_window.styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskResizable;

    WindowDelegate *delegate = [[WindowDelegate alloc]
            initWithMacWindow:mac_window];
    mac_window.delegate = delegate;

    window->window_impl = (void *) CFBridgingRetain(delegate);
}

void os_window_destroy(OsWindow *window)
{
    CFRelease(window->window_impl);
}

void os_window_set_view(OsWindow *window, GpuView *view)
{
    WindowDelegate *delegate = (__bridge WindowDelegate *) window->window_impl;
    ViewDelegate *view_delegate = (__bridge ViewDelegate *) view->view_impl;
    delegate.view = view_delegate.mtk_view;
    delegate.window.contentViewController = delegate;
}

float2 os_window_size(OsWindow *window)
{
    WindowDelegate *delegate = (__bridge WindowDelegate *) window->window_impl;
    NSSize size = [delegate.window contentView].frame.size;
    return (float2) {
        size.width,
        size.height,
    };
}

void os_window_show(OsWindow *window)
{
    WindowDelegate *delegate = (__bridge WindowDelegate *) window->window_impl;
    MacWindow *mac_window = delegate.window;
    [mac_window makeKeyAndOrderFront:delegate];
    [mac_window toggleFullScreen:delegate];
}
