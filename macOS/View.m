@import MetalKit;

#import "spacetime.h"
#import "View.h"

const double VIEW_ENTER_DRAG_TIME = 0.1;
const double VIEW_ENTER_DRAG_DELTA_SQUARED = 30.0;

@implementation View {
    BOOL _dragging;
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

                if (time - startTime >= VIEW_ENTER_DRAG_TIME ||
                        delta.x * delta.x + delta.y * delta.y >=
                        VIEW_ENTER_DRAG_DELTA_SQUARED) {
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

- (void)mouseUp:(NSEvent *)event {
    (void)event;
    _dragging = NO;
}

@end
