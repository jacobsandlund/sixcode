#ifndef Log_h
#define Log_h

#include "spacetime.h"

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

Log *log_create(LogConfig *config);
void log_destroy(Log *log);
void log_at_level(Log *log, LogLevel level, const char *format, ...);

#endif // Log_h
