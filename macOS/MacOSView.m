@import MetalKit;

#import "spacetime.h"
#import "MacOSView.h"
#import "view.h"

const double MACOS_VIEW_ENTER_DRAG_TIME = 0.1;
const double MACOS_VIEW_ENTER_DRAG_DELTA_SQUARED = 30.0;

void view_initialize(View *vw, float2 viewport_size)
{
    vw->viewport_size = viewport_size;
    id<MTLDevice> device = MTLCreateSystemDefaultDevice();

    if (!device) {
        vw->os_view = NULL;
        NSLog(@"Metal is not supported on this device");
        return;
    }

    MacOSView *macOSView = [[MacOSView alloc] initWithView:vw device:device];

    vw->os_view = (void *) CFBridgingRetain(macOSView);
}

void view_terminate(View *vw)
{
    CFRelease(vw->os_view);
}

@implementation MacOSView {
    View *_view;
    BOOL _dragging;
}

- (instancetype) initWithView:(View *)vw device:(id<MTLDevice>)device {
    NSRect frame = NSMakeRect(0.0, 0.0, vw->viewport_size.x, vw->viewport_size.y);
    self = [super initWithFrame:frame device:device];
    if (self) {
        _view = vw;
    }
    return self;
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (void)mouseDown:(NSEvent *)event {
    NSTimeInterval startTime = event.timestamp;
    NSPoint startLocation = event.locationInWindow;

    while (YES) {
        NSTimeInterval time;
        NSPoint location;
        event = [self.window nextEventMatchingMask: NSEventMaskLeftMouseUp |
                NSEventMaskLeftMouseDragged];

        switch (event.type) {
            case NSEventTypeLeftMouseDragged:
                time = event.timestamp;
                location = event.locationInWindow;
                double2 delta = {
                    location.x - startLocation.x,
                    location.y - startLocation.y,
                };

                if (time - startTime >= MACOS_VIEW_ENTER_DRAG_TIME ||
                        delta.x * delta.x + delta.y * delta.y >=
                        MACOS_VIEW_ENTER_DRAG_DELTA_SQUARED) {
                    _dragging = YES;
                    NSLog(@"Mouse enter drag - loc: %g, %g",
                            location.x, location.y);
                    return;
                }

                break;

            case NSEventTypeLeftMouseUp:
                _dragging = NO;
                return;

            default:
                break;
        }
    };
}

- (void)mouseMoved:(NSEvent *)event {
    (void)event;
    NSLog(@"mouseMoved");
}

- (void)mouseUp:(NSEvent *)event {
    (void)event;
    _dragging = NO;
}

@end
