#include "Log/Manager.h"

LogManager gLogManager;

void LogManagerInit(LogManagerConfig *config)
{
    gLogManager.logs.os = LogCreate(&config->logs.os);
    gLogManager.logs.gpu = LogCreate(&config->logs.gpu);
    gLogManager.logs.engine = LogCreate(&config->logs.engine);
}

void log_manager_destroy(void)
{
    LogDestroy(gLogManager.logs.os);
    LogDestroy(gLogManager.logs.gpu);
    LogDestroy(gLogManager.logs.engine);
}
