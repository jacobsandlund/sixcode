#include "log/manager.h"

LogManager gLogManager;

void log_manager_init(LogManagerConfig *config)
{
    log_init(&gLogManager.logs.os, &config->logs.os);
    log_init(&gLogManager.logs.gpu, &config->logs.gpu);
    log_init(&gLogManager.logs.engine, &config->logs.engine);
}

void log_manager_destroy(void)
{
    log_destroy(&gLogManager.logs.os);
    log_destroy(&gLogManager.logs.gpu);
    log_destroy(&gLogManager.logs.engine);
}
