#ifndef _EngineProfile_h
#define _EngineProfile_h

#include "spacetime.h"
#include "os/clock.h"
#include "log/manager.h"

#ifdef DEBUG

#define ENGINE_PROFILE_START() \
    u64 end_time; \
    u64 start_time = OsClockTime();

#define ENGINE_PROFILE_END(block_name) { \
    end_time = OsClockTime(); \
    LogDebug(gLogManager.logs.engine, "%s in %" PRIu64 " us",\
            block_name, (end_time - start_time + 500) / 1000); \
    start_time = end_time; \
}

#else

#define ENGINE_PROFILE_START() /* */
#define ENGINE_PROFILE_END() /* */

#endif // DEBUG


#endif // _EngineProfile_h
