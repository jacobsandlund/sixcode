#include "log/manager.h"

LogManager gLogManager;

void log_manager_init(LogManagerConfig *config)
{
    gLogManager.logs.os = log_create(&config->logs.os);
    gLogManager.logs.gpu = log_create(&config->logs.gpu);
    gLogManager.logs.engine = log_create(&config->logs.engine);
}

void log_manager_destroy(void)
{
    log_destroy(gLogManager.logs.os);
    log_destroy(gLogManager.logs.gpu);
    log_destroy(gLogManager.logs.engine);
}
