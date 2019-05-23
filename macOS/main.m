#import <stdlib.h>
#import <time.h>
#import "AppDelegate.h"
#import "event-queue.h"

const i64 EVENT_QUEUE_LENGTH = 64;
const double ENTER_DRAG_TIME = 0.1;
const double ENTER_DRAG_DELTA_SQUARED = 30.0;

NSMenu *makeMenu() {
    NSMenu *mainMenu = [[NSMenu alloc] init];
    NSMenuItem *mainSpacetimeMenuItem = [[NSMenuItem alloc]
            initWithTitle:@"Spacetime" action:nil keyEquivalent:@""];
    [mainMenu addItem:mainSpacetimeMenuItem];

    NSMenu *mainSpacetimeMenu = [[NSMenu alloc] init];
    mainSpacetimeMenuItem.submenu = mainSpacetimeMenu;

    [mainSpacetimeMenu addItemWithTitle:@"Quit Spacetime"
            action:@selector(terminate:) keyEquivalent:@"q"];

    return mainMenu;
}

void runLoop(EventQueue *eq) {
    NSDate *until = [NSDate distantFuture];

    u64 leftMouseDownTime = 0.0;
    float2 leftMouseDownLocation = {0.0f, 0.0f};
    bool dragging = false;

    for (;;) {

        @autoreleasepool {

        NSEvent *nsEvent = [NSApp nextEventMatchingMask:NSEventMaskAny
                untilDate:until inMode:NSDefaultRunLoopMode
                dequeue:YES];

        Event event = {
            .time = clock_gettime_nsec_np(CLOCK_UPTIME_RAW),
        };

        switch (nsEvent.type) {

        case NSEventTypeLeftMouseDown:
            leftMouseDownTime = event.time;
            leftMouseDownLocation = (float2) {
                (float) nsEvent.locationInWindow.x,
                (float) nsEvent.locationInWindow.y,
            };

            break;

        case NSEventTypeLeftMouseUp:
            if (!dragging) {
                event.type = EVENT_TYPE_MOUSE_CLICK;
                event.location = (float2) {
                    (float) nsEvent.locationInWindow.x,
                    (float) nsEvent.locationInWindow.y,
                };
                event_queue_write(eq, &event);
            }

            dragging = false;

            break;

        case NSEventTypeMouseMoved:
            event.type = EVENT_TYPE_MOUSE_MOVE;
            event.location = (float2) {
                (float) nsEvent.locationInWindow.x,
                (float) nsEvent.locationInWindow.y,
            };
            event_queue_write(eq, &event);

            break;

        case NSEventTypeLeftMouseDragged:
            event.location = (float2) {
                (float) nsEvent.locationInWindow.x,
                (float) nsEvent.locationInWindow.y,
            };
            float2 delta = {
                event.location.x - leftMouseDownLocation.x,
                event.location.y - leftMouseDownLocation.y,
            };

            if (dragging ||
                    event.time - leftMouseDownTime >= ENTER_DRAG_TIME ||
                    delta.x * delta.x + delta.y * delta.y >=
                    ENTER_DRAG_DELTA_SQUARED) {
                dragging = true;
                event.type = EVENT_TYPE_MOUSE_DRAG;
                event_queue_write(eq, &event);
            }

            break;

        case NSEventTypeKeyDown:
            if (nsEvent.modifierFlags & NSEventModifierFlagCommand) {
                [NSApp sendEvent:nsEvent];
                return;
            }

            NSLog(@"Event key down with characters: '%@' - %@", nsEvent.characters, nsEvent);

            break;

        case NSEventTypeKeyUp:
            NSLog(@"Event key up with characters: '%@' - %@", nsEvent.characters, nsEvent);

            break;

        case NSEventTypeFlagsChanged:
            NSLog(@"Event flags changed: %@", nsEvent);

            break;

        case NSEventTypeAppKitDefined:
            switch ((i16) nsEvent.subtype) {
            case NSEventSubtypeApplicationActivated:
                NSLog(@"Event with subtype: NSEventSubtypeApplicationActivated - %@", nsEvent);
                break;
            case NSEventSubtypeApplicationDeactivated:
                NSLog(@"Event with subtype: NSEventSubtypeApplicationDeactivated - %@", nsEvent);
                break;
            case NSEventSubtypeScreenChanged:
                NSLog(@"Event with subtype: NSEventSubtypeScreenChanged - %@", nsEvent);
                break;
            case NSEventSubtypeWindowExposed:
                NSLog(@"Event with subtype: NSEventSubtypeWindowExposed - %@", nsEvent);
                break;
            case NSEventSubtypeWindowMoved:
                NSLog(@"Event with subtype: NSEventSubtypeWindowMoved - %@", nsEvent);
                break;
            case 9:
                NSLog(@"Event with subtype 9: activated ??? - %@", nsEvent);
                break;
            case 22:
                NSLog(@"Event with subtype 22: booting up ??? - %@", nsEvent);
                break;
            case 23:
                NSLog(@"Event with subtype 23: booting up ??? - %@", nsEvent);
                break;
            default:
                NSLog(@"Event with unknown subtype: %d - %@", nsEvent.subtype, nsEvent);
                break;
            }

            [NSApp sendEvent:nsEvent];

            break;

        case NSEventTypeSystemDefined:
        case NSEventTypeApplicationDefined:
        case NSEventTypePeriodic:
            switch ((i16) nsEvent.subtype) {
            case NSEventSubtypePowerOff:
            // case NSEventSubtypeApplicationActivated:
            // case NSEventSubtypeTabletPoint:
                NSLog(@"Event with subtype: NSEventSubtypePowerOff - %@", nsEvent);
                break;
            case NSEventSubtypeScreenChanged:
                NSLog(@"Event with subtype: NSEventSubtypeScreenChanged - %@", nsEvent);
                break;
            case NSEventSubtypeTouch:
                NSLog(@"Event with subtype: NSEventSubtypeTouch - %@", nsEvent);
                break;
            case NSEventSubtypeMouseEvent:
            // case NSEventSubtypeTabletProximity:
                NSLog(@"Event with subtype: NSEventSubtypeMouseEvent - %@", nsEvent);
                break;
            case 7:
                NSLog(@"Event with subtype 7: clicking into/out of ??? - %@", nsEvent);
                break;
            default:
                NSLog(@"Event with unknown subtype: %d - %@", nsEvent.subtype, nsEvent);
                break;
            }

            [NSApp sendEvent:nsEvent];

            break;

        default:
            NSLog(@"Event other: %@", nsEvent);
            [NSApp sendEvent:nsEvent];

            break;
        }

        } // @autoreleasepool
    }
}


int main(int argc, const char *argv[]) {
    (void)argc;
    (void)argv;

    @autoreleasepool {

    [NSApplication sharedApplication];

    EventQueue *eq = malloc(sizeof *eq);
    event_queue_initialize(eq, EVENT_QUEUE_LENGTH);

    AppDelegate *appDelegate = [[AppDelegate alloc] initWithEventQueue:eq];
    NSApp.mainMenu = makeMenu();
    NSApp.presentationOptions = NSApplicationPresentationHideMenuBar | NSApplicationPresentationHideDock;
    [NSApp setDelegate:appDelegate];
    [NSApp finishLaunching];

    runLoop(eq);

    event_queue_terminate(eq);
    free(eq);

    } // @autoreleasepool

    return 0;
}
