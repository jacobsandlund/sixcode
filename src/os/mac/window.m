@import AppKit

#import "os/window.h"

@interface WindowDelegate : NSViewController<NSWindowDelegate>
@end

@implementation WindowDelegate
@end

void os_window_init(OsWindow *window)
{
    NSWindow *ns_window = [[NSWindow alloc] init];
    ns_window.styleMask = NSWindowStyleMaskTitled |
            NSWindowStyleMaskResizable | NSWindowStyleMaskFullScreen;

    WindowDelegate *delegate = [[WindowDelegate alloc] init];
    [delegate retain];
    ns_window.delegate = delegate;

    window->window_impl = (void *) CFBridgingRetain(ns_window);
}

void os_window_destroy(OsWindow *window)
{
    NSWindow *ns_window = (__bridge NSWindow *) window->window_impl;
    WindowDelegate *delegate = (WindowDelegate *) ns_window->delegate;
    [delegate release];
    CFRelease(window->window_impl);
}

void os_window_set_view(OsWindow *window, GpuView *view)
{
    NSWindow *ns_window = (__bridge NSWindow *) window->window_impl;
    WindowDelegate *delegate = (WindowDelegate *) ns_window->delegate;
    NSView *ns_view = (__bridge NSView *) view->view_impl;
    delegate.view = ns_view;
    ns_window.contentViewController = delegate;
}

float2 os_window_size(OsWindow *window)
{
    NSWindow *ns_window = (__bridge NSWindow *) window->window_impl;
    NSSize size = [ns_window contentView].frame.size;
    return (float2) {
        size.width,
        size.height,
    };
}

void os_window_show(OsWindow *window)
{
    NSWindow *ns_window = (__bridge WindowDelegate *) window->window_impl;
    [ns_window makeKeyAndOrderFront:ns_window];
}
