#import "Os/EventLoop.h"

@import AppKit;

#import <stdarg.h>
#import <stdlib.h>
#import "Log/Manager.h"
#import "Os/Clock.h"

const double EnterDragTime = 0.1;
const double EnterDragDeltaSquared = 30.0;

struct OsEventLoop {
    u64 left_mouse_down_time;
    float2 left_mouse_down_location;
    bool left_mouse_dragging;
};

OsEventLoop *OsEventLoopCreate(void)
{
    OsEventLoop *loop = malloc(sizeof *loop);

    loop->left_mouse_down_time = 0.0;
    loop->left_mouse_down_location = (float2) {0.0f, 0.0f};
    loop->left_mouse_dragging = false;

    return loop;
}

void OsEventLoopDestroy(OsEventLoop *loop)
{
    free(loop);
}

void OsEventLoopRun(OsEventLoop *loop, OsEventQueue *queue)
{
    @autoreleasepool {

    NSDate *until = [NSDate distantFuture];

    for (;;) {

        @autoreleasepool {

        NSEvent *nsEvent = [NSApp nextEventMatchingMask:NSEventMaskAny
                untilDate:until inMode:NSDefaultRunLoopMode
                dequeue:YES];

        OsEvent event = {
            .time = OsClockTime(),
        };

        switch (nsEvent.type) {

        case NSEventTypeLeftMouseDown:
            LogDebug(gLogManager.logs.os, "Event left mouse down");
            loop->left_mouse_down_time = event.time;
            loop->left_mouse_down_location = (float2) {
                (float) nsEvent.locationInWindow.x,
                (float) nsEvent.locationInWindow.y,
            };

            [NSApp sendEvent:nsEvent];
            [NSApp updateWindows];

            break;

        case NSEventTypeLeftMouseUp:
            LogDebug(gLogManager.logs.os, "Event left mouse up");
            if (!loop->left_mouse_dragging) {
                event.type = OsEventTypeMouseClick;
                event.location = (float2) {
                    (float) nsEvent.locationInWindow.x,
                    (float) nsEvent.locationInWindow.y,
                };
                OsEventQueueWrite(queue, &event);
            }

            loop->left_mouse_dragging = false;

            break;

        case NSEventTypeMouseMoved:
            // Don't spam logs

            event.type = OsEventTypeMouseMove;
            event.location = (float2) {
                (float) nsEvent.locationInWindow.x,
                (float) nsEvent.locationInWindow.y,
            };
            OsEventQueueWrite(queue, &event);

            break;

        case NSEventTypeLeftMouseDragged:
            LogDebug(gLogManager.logs.os, "Event left mouse dragged");
            event.location = (float2) {
                (float) nsEvent.locationInWindow.x,
                (float) nsEvent.locationInWindow.y,
            };
            float2 delta = {
                event.location.x - loop->left_mouse_down_location.x,
                event.location.y - loop->left_mouse_down_location.y,
            };

            if (loop->left_mouse_dragging ||
                    event.time - loop->left_mouse_down_time >= EnterDragTime ||
                    delta.x * delta.x + delta.y * delta.y >=
                    EnterDragDeltaSquared) {
                loop->left_mouse_dragging = true;
                event.type = OsEventTypeMouseDrag;
                OsEventQueueWrite(queue, &event);
            }

            break;

        case NSEventTypeKeyDown:
            if (nsEvent.modifierFlags & NSEventModifierFlagCommand) {
                [NSApp sendEvent:nsEvent];
                [NSApp updateWindows];
            }

            LogDebug(gLogManager.logs.os,
                    "Event key down with characters: '%s'", nsEvent.characters.UTF8String);

            break;

        case NSEventTypeKeyUp:
            LogDebug(gLogManager.logs.os,
                    "Event key up with characters: '%s'", nsEvent.characters.UTF8String);

            break;

        case NSEventTypeFlagsChanged:
            LogDebug(gLogManager.logs.os, "Event flags changed");

            break;

        case NSEventTypeAppKitDefined:
            switch ((i16) nsEvent.subtype) {
            case NSEventSubtypeApplicationActivated:
                LogDebug(gLogManager.logs.os,
                        "Event NSEventTypeAppKitDefined with subtype: NSEventSubtypeApplicationActivated");
                break;
            case NSEventSubtypeApplicationDeactivated:
                LogDebug(gLogManager.logs.os,
                        "Event NSEventTypeAppKitDefined with subtype: NSEventSubtypeApplicationDeactivated");
                break;
            case NSEventSubtypeScreenChanged:
                LogDebug(gLogManager.logs.os,
                        "Event NSEventTypeAppKitDefined with subtype: NSEventSubtypeScreenChanged");
                break;
            case NSEventSubtypeWindowExposed:
                LogDebug(gLogManager.logs.os,
                        "Event NSEventTypeAppKitDefined with subtype: NSEventSubtypeWindowExposed");
                break;
            case NSEventSubtypeWindowMoved:
                LogDebug(gLogManager.logs.os,
                        "Event NSEventTypeAppKitDefined with subtype: NSEventSubtypeWindowMoved");
                break;
            case 9:
                LogDebug(gLogManager.logs.os,
                        "Event NSEventTypeAppKitDefined with subtype 9: activated ???");
                break;
            case 22:
                LogDebug(gLogManager.logs.os,
                        "Event NSEventTypeAppKitDefined with subtype 22: booting up ???");
                break;
            case 23:
                LogDebug(gLogManager.logs.os,
                        "Event NSEventTypeAppKitDefined with subtype 23: booting up ???");
                break;
            default:
                LogDebug(gLogManager.logs.os,
                        "Event NSEventTypeAppKitDefined with unknown subtype: %d", nsEvent.subtype);
                break;
            }

            [NSApp sendEvent:nsEvent];
            [NSApp updateWindows];

            break;

        case NSEventTypeSystemDefined:
        case NSEventTypeApplicationDefined:
        case NSEventTypePeriodic:
            switch ((i16) nsEvent.subtype) {
            case NSEventSubtypePowerOff:
            // case NSEventSubtypeTabletPoint:
                LogDebug(gLogManager.logs.os,
                        "Event NSEventTypeSystemDefined with subtype: NSEventSubtypePowerOff");
                break;
            case NSEventSubtypeScreenChanged:
                LogDebug(gLogManager.logs.os,
                        "Event NSEventTypeSystemDefined with subtype: NSEventSubtypeScreenChanged");
                break;
            case NSEventSubtypeTouch:
                LogDebug(gLogManager.logs.os,
                        "Event NSEventTypeSystemDefined with subtype: NSEventSubtypeTouch");
                break;
            case NSEventSubtypeMouseEvent:
            // case NSEventSubtypeTabletProximity:
                LogDebug(gLogManager.logs.os,
                        "Event NSEventTypeSystemDefined with subtype: NSEventSubtypeMouseEvent");
                break;
            case 7:
                LogDebug(gLogManager.logs.os,
                        "Event NSEventTypeSystemDefined with subtype 7: clicking into/out of ???");
                break;
            default:
                LogDebug(gLogManager.logs.os,
                        "Event NSEventTypeSystemDefined with unknown subtype");
                break;
            }

            [NSApp sendEvent:nsEvent];
            [NSApp updateWindows];

            break;

        default:
            LogDebug(gLogManager.logs.os,
                    "Event other with type: %d", nsEvent.type);

            [NSApp sendEvent:nsEvent];
            [NSApp updateWindows];

            break;
        }

        } // @autoreleasepool
    }

    } // @autoreleasepool
}
