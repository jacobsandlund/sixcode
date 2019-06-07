const double EnterDragTime = 0.1;
const double EnterDragDeltaSquared = 30.0;

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
            .time = event_queue_clock_time(),
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
                event.type = EventTypeMouseClick;
                event.location = (float2) {
                    (float) nsEvent.locationInWindow.x,
                    (float) nsEvent.locationInWindow.y,
                };
                event_queue_write(eq, &event);
            }

            dragging = false;

            break;

        case NSEventTypeMouseMoved:
            event.type = EventTypeMouseMove;
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
                    event.time - leftMouseDownTime >= EnterDragTime ||
                    delta.x * delta.x + delta.y * delta.y >=
                    EnterDragDeltaSquared) {
                dragging = true;
                event.type = EventTypeMouseDrag;
                event_queue_write(eq, &event);
            }

            break;

        case NSEventTypeKeyDown:
            if (nsEvent.modifierFlags & NSEventModifierFlagCommand) {
                [NSApp sendEvent:nsEvent];
                return;
            }

            DLog("Event key down with characters: '%{public}@' - %{public}@", nsEvent.characters, nsEvent);

            break;

        case NSEventTypeKeyUp:
            DLog("Event key up with characters: '%{public}@' - %{public}@", nsEvent.characters, nsEvent);

            break;

        case NSEventTypeFlagsChanged:
            DLog("Event flags changed: %{public}@", nsEvent);

            break;

        case NSEventTypeAppKitDefined:
            switch ((i16) nsEvent.subtype) {
            case NSEventSubtypeApplicationActivated:
                DLog("Event with subtype: NSEventSubtypeApplicationActivated - %{public}@", nsEvent);
                break;
            case NSEventSubtypeApplicationDeactivated:
                DLog("Event with subtype: NSEventSubtypeApplicationDeactivated - %{public}@", nsEvent);
                break;
            case NSEventSubtypeScreenChanged:
                DLog("Event with subtype: NSEventSubtypeScreenChanged - %{public}@", nsEvent);
                break;
            case NSEventSubtypeWindowExposed:
                DLog("Event with subtype: NSEventSubtypeWindowExposed - %{public}@", nsEvent);
                break;
            case NSEventSubtypeWindowMoved:
                DLog("Event with subtype: NSEventSubtypeWindowMoved - %{public}@", nsEvent);
                break;
            case 9:
                DLog("Event with subtype 9: activated ??? - %{public}@", nsEvent);
                break;
            case 22:
                DLog("Event with subtype 22: booting up ??? - %{public}@", nsEvent);
                break;
            case 23:
                DLog("Event with subtype 23: booting up ??? - %{public}@", nsEvent);
                break;
            default:
                DLog("Event with unknown subtype: %d - %{public}@", nsEvent.subtype, nsEvent);
                break;
            }

            [NSApp sendEvent:nsEvent];

            break;

        case NSEventTypeSystemDefined:
        case NSEventTypeApplicationDefined:
        case NSEventTypePeriodic:
            switch ((i16) nsEvent.subtype) {
            case NSEventSubtypePowerOff:
            // case NSEventSubtypeTabletPoint:
                DLog("Event with subtype: NSEventSubtypePowerOff - %{public}@", nsEvent);
                break;
            case NSEventSubtypeScreenChanged:
                DLog("Event with subtype: NSEventSubtypeScreenChanged - %{public}@", nsEvent);
                break;
            case NSEventSubtypeTouch:
                DLog("Event with subtype: NSEventSubtypeTouch - %{public}@", nsEvent);
                break;
            case NSEventSubtypeMouseEvent:
            // case NSEventSubtypeTabletProximity:
                DLog("Event with subtype: NSEventSubtypeMouseEvent - %{public}@", nsEvent);
                break;
            case 7:
                DLog("Event with subtype 7: clicking into/out of ??? - %{public}@", nsEvent);
                break;
            default:
                DLog("Event with unknown subtype: %d - %{public}@", nsEvent.subtype, nsEvent);
                break;
            }

            [NSApp sendEvent:nsEvent];

            break;

        default:
            DLog("Event other: %{public}@", nsEvent);
            [NSApp sendEvent:nsEvent];

            break;
        }

        } // @autoreleasepool
    }
}
