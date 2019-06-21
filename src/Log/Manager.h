#ifndef _LogManager_h
#define _LogManager_h

#include "Spacetime.h"
#include "Log/Log.h"

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
#define LogDebug(log, ...) LogAtLevel(log, LogLevelDebug, __VA_ARGS__)
#else
#define LogDebug(log, ...) /* */
#endif

#define LogDefault(log, ...) LogAtLevel(log, LogLevelDefault, __VA_ARGS__)
#define LogInfo(log, ...) LogAtLevel(log, LogLevelInfo, __VA_ARGS__)
#define LogError(log, ...) LogAtLevel(log, LogLevelError, __VA_ARGS__)

void LogManagerInit(LogManagerConfig *config);
void log_manager_destroy(void);

#endif // _LogManager_h
