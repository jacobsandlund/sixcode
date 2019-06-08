#include "log/manager.h"

LogManager gLogManager;

void log_manager_init(LogManagerConfig *config)
{
    log_init(&gLogManager.logs.log, &config->logs.log);
    log_init(&gLogManager.logs.os, &config->logs.os);
    log_init(&gLogManager.logs.gpu, &config->logs.gpu);
    log_init(&gLogManager.logs.string, &config->logs.string);
    log_init(&gLogManager.logs.resource, &config->logs.resource);
    log_init(&gLogManager.logs.render, &config->logs.render);
    log_init(&gLogManager.logs.camera, &config->logs.camera);
    log_init(&gLogManager.logs.world, &config->logs.world);
}

void log_manager_destroy(void)
{
    log_destroy(&gLogManager.logs.log);
    log_destroy(&gLogManager.logs.os);
    log_destroy(&gLogManager.logs.gpu);
    log_destroy(&gLogManager.logs.string);
    log_destroy(&gLogManager.logs.resource);
    log_destroy(&gLogManager.logs.render);
    log_destroy(&gLogManager.logs.camera);
    log_destroy(&gLogManager.logs.world);
}
