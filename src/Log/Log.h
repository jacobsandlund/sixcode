#ifndef _Log_h
#define _Log_h

#include "Spacetime.h"

typedef enum {
    LogLevelDefault = 0,
    LogLevelInfo,
    LogLevelDebug,
    LogLevelError,
} LogLevel;

typedef struct {
    const char *subsystem;
    const char *category;
} LogConfig;

typedef struct Log Log;

Log *LogCreate(LogConfig *config);
void LogDestroy(Log *log);
void LogAtLevel(Log *log, LogLevel level, const char *format, ...);

#endif  // _Log_h
