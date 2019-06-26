#include "Engine/Manager.h"

#include "Engine/Profile.h"
#include "Os/Screen.h"
#include "Render/Layout.h"

EngineManagerCallbacks gEngineManagerCallbacks = {
    .os = {
        .application = {
            .will_terminate = EngineManagerDestroy,
        },
    },
    .gpu = {
        .view = {
            .draw_in_view = GpuManagerDrawInView,
            .size_changed = GpuManagerSizeChanged,
        },
    },
};

EngineManagerConfig gEngineManagerConfig = {
    .log = {
        .logs = {
            .os = {
                .subsystem = "computer.spacetime.os",
                .category = "default",
            },
            .gpu = {
                .subsystem = "computer.spacetime.gpu",
                .category = "default",
            },
            .engine = {
                .subsystem = "computer.spacetime.engine",
                .category = "default",
            },
        },
    },
    .os = {
        .event_queue_length = 64,
        .event_queue_safe_length_remaining = 16,
    },
    .gpu = {
        .view = {
            .preferred_frames_per_second = 60,
            .color_pixel_format = GpuPixelFormatBGRA8Unorm_sRGB,
        },
    },
    .render = {
        .layout_type = RenderLayoutTypeHex,
    },
    .camera = {
        .position = {0.0, 0.0, 32.0},
    },
    .world = {
        .grid_size = 4096,
    },
    .world_grid_random_count = 10000000,
};

void EngineManagerInit(EngineManagerCallbacks *callbacks,
                       EngineManagerConfig *config)
{
    DEBUG_EngineProfileStart();

    LogManagerInit(&config->log);
    DEBUG_EngineProfileEnd("LogManagerInit");

    OsManagerInit(&config->os);
    DEBUG_EngineProfileEnd("OsManagerInit");

    GpuManagerInit(OsScreenVisibleFrame(), &config->gpu);
    DEBUG_EngineProfileEnd("GpuManagerInit");

    OsManagerWindowInit(gGpuManager.view);
    DEBUG_EngineProfileEnd("OsManagerWindowInit");

    RenderManagerInit(&config->render);
    DEBUG_EngineProfileEnd("RenderManagerInit");

    CameraManagerInit(&config->camera);
    DEBUG_EngineProfileEnd("CameraManagerInit");

    WorldManagerInit(&config->world);
    DEBUG_EngineProfileEnd("WorldManagerInit");

    GpuManagerRegisterCallbacks(&callbacks->gpu);
    DEBUG_EngineProfileEnd("GpuManagerRegisterCallbacks");

    OsManagerRegisterCallbacks(&callbacks->os);
    DEBUG_EngineProfileEnd("OsManagerRegisterCallbacks");

    OsManagerFinishLaunching();
    DEBUG_EngineProfileEnd("OsManagerFinishLaunching");

    WorldManagerLoadRandom(config->world_grid_random_count);
    DEBUG_EngineProfileEnd("WorldManagerLoadRandom");

    DEBUG_EngineProfileEnd("EngineProfile empty timing 1");
    DEBUG_EngineProfileEnd("EngineProfile empty timing 2");
}

void EngineManagerDestroy(void)
{
    DEBUG_EngineProfileStart();

    WorldManagerDestroy();
    DEBUG_EngineProfileEnd("WorldManagerDestroy");

    CameraManagerDestroy();
    DEBUG_EngineProfileEnd("CameraManagerDestroy");

    RenderManagerDestroy();
    DEBUG_EngineProfileEnd("RenderManagerDestroy");

    OsManagerWindowDestroy();
    DEBUG_EngineProfileEnd("OsManagerWindowDestroy");

    GpuManagerDestroy();
    DEBUG_EngineProfileEnd("GpuManagerDestroy");

    OsManagerDestroy();
    DEBUG_EngineProfileEnd("OsManagerDestroy");

    log_manager_destroy();
    // No more logging allowed
}

void EngineManagerRun(void)
{
    OsManagerEventLoopRun();
}
