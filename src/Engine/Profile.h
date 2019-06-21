#ifndef _EngineProfile_h
#define _EngineProfile_h

#include "Spacetime.h"
#include "Os/Clock.h"
#include "Log/Manager.h"

#ifdef DEBUG

#define DEBUG_EngineProfileStart() \
    u64 end_time; \
    u64 start_time = OsClockTime();

#define DEBUG_EngineProfileEnd(block_name) { \
    end_time = OsClockTime(); \
    LogDebug(gLogManager.logs.engine, "%s in %" PRIu64 " us",\
            block_name, (end_time - start_time + 500) / 1000); \
    start_time = end_time; \
}

#else

#define DEBUG_EngineProfileStart() /* */
#define DEBUG_EngineProfileEnd() /* */

#endif // DEBUG


#endif // _EngineProfile_h
