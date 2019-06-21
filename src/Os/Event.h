#ifndef _OsEvent_h
#define _OsEvent_h

typedef enum {
    OsEventTypeMouseMove = 0x00000001,
    OsEventTypeMouseDrag = 0x00000002,
    OsEventTypeMouseClick = 0x00000004,

    // Used for mock event loop to end looping
    OsEventTypeTerminateLoop,
} OsEventType;

enum { OsEventTypeMouseMoveOrDrag = 3 };

typedef struct {
    OsEventType type;
    u64 time;
    float2 location;
} OsEvent;

#endif // _OsEvent_h
