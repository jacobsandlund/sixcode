#ifndef LogManager_h
#define LogManager_h

#include "spacetime.h"
#include "log/log.h"

typedef struct {
    LogConfig log;
    LogConfig os;
    LogConfig gpu;
    LogConfig string;
    LogConfig resource;
    LogConfig render;
    LogConfig camera;
    LogConfig world;
} LogManagerLogsConfig;

typedef struct {
    LogManagerLogsConfig logs;
} LogManagerConfig;

typedef struct {
    Log log;
    Log os;
    Log gpu;
    Log string;
    Log resource;
    Log render;
    Log camera;
    Log world;
} LogManagerLogs;

typedef struct {
    LogManagerLogs logs;
} LogManager;

extern LogManager gLogManager;

void log_manager_init(LogManagerConfig *config);
void log_manager_destroy(void);

#endif // LogManager_h
