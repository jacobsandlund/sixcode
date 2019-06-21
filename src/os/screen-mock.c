#include "os/screen.h"

OsScreenFrame gOsScreenFrameMock;

void os_screen_mock_init(float2 size)
{
    gOsScreenFrameMock.origin = (float2) {0, 0};
    gOsScreenFrameMock.size = size;
}

OsScreenFrame OsScreenVisibleFrame(void)
{
    return gOsScreenFrameMock;
}
