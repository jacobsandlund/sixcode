#ifndef OsEvent_h
#define OsEvent_h

typedef enum {
    OsEventTypeMouseMove = 1,
    OsEventTypeMouseDrag = 2,
    OsEventTypeMouseClick = 4,

    // Used for mock event loop to end looping
    OsEventTypeTerminateLoop,
} OsEventType;

#define OsEventTypeMouseMoveOrDrag 3

typedef struct {
    OsEventType type;
    u64 time;
    float2 location;
} OsEvent;

#endif // OsEvent_h
