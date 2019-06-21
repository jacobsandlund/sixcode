@import AppKit;

#import "Os/Screen.h"

OsScreenFrame OsScreenVisibleFrame(void)
{
    NSRect frame = [NSScreen mainScreen].visibleFrame;
    return (OsScreenFrame) {
        .origin = {
            frame.origin.x,
            frame.origin.y,
        },
        .size = {
            frame.size.width,
            frame.size.height,
        },
    };
}
