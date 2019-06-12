#ifndef LogManager_h
#define LogManager_h

#include "spacetime.h"
#include "log/log.h"

typedef struct {
    LogConfig os;
    LogConfig gpu;
    LogConfig engine;
} LogManagerLogsConfig;

typedef struct {
    LogManagerLogsConfig logs;
} LogManagerConfig;

typedef struct {
    Log *os;
    Log *gpu;
    Log *engine;
} LogManagerLogs;

typedef struct {
    LogManagerLogs logs;
} LogManager;

extern LogManager gLogManager;

#ifdef DEBUG
#define LogDebug(log, ...) log_at_level(log, LogLevelDebug, __VA_ARGS__)
#else
#define LogDebug(log, ...) /* */
#endif

#define LogDefault(log, ...) log_at_level(log, LogLevelDefault, __VA_ARGS__)
#define LogInfo(log, ...) log_at_level(log, LogLevelInfo, __VA_ARGS__)
#define LogError(log, ...) log_at_level(log, LogLevelError, __VA_ARGS__)

void log_manager_init(LogManagerConfig *config);
void log_manager_destroy(void);

#endif // LogManager_h
