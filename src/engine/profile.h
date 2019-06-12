#ifndef EngineProfile_h
#define EngineProfile_h

#include "spacetime.h"
#include "os/clock.h"
#include "log/manager.h"

#ifdef DEBUG

#define EngineProfileStart() \
    u64 end_time; \
    u64 start_time = os_clock_time();

#define EngineProfileEnd(block_name) { \
    end_time = os_clock_time(); \
    LogDebug(gLogManager.logs.engine, "%s in %" PRIu64 " us",\
            block_name, (end_time - start_time + 500) / 1000); \
    start_time = end_time; \
}

#else

#define EngineProfileStart() /* */
#define EngineProfileEnd() /* */

#endif // DEBUG


#endif // EngineProfile_h
