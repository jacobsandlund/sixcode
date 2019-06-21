#include "engine/manager.h"
#include "engine/profile.h"
#include "os/screen.h"
#include "render/layout.h"

EngineCallbacks gEngineManagerCallbacks = {
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

EngineConfig gEngineManagerConfig = {
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
            .color_pixel_format = MTLPixelFormatBGRA8Unorm_sRGB,
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

void EngineManagerInit(EngineCallbacks *callbacks, EngineConfig *config)
{
        ENGINE_PROFILE_START();
    LogManagerInit(&config->log);
        ENGINE_PROFILE_END("LogManagerInit");
    OsManagerInit(&config->os);
        ENGINE_PROFILE_END("OsManagerInit");

    GpuManagerInit(OsScreenVisibleFrame(), &config->gpu);
        ENGINE_PROFILE_END("GpuManagerInit");

    OsManagerWindowInit(gGpuManager.view);
        ENGINE_PROFILE_END("OsManagerWindowInit");

    RenderManagerInit(&config->render);
        ENGINE_PROFILE_END("RenderManagerInit");
    CameraManagerInit(&config->camera);
        ENGINE_PROFILE_END("CameraManagerInit");
    WorldManagerInit(&config->world);
        ENGINE_PROFILE_END("WorldManagerInit");
    GpuManagerRegisterCallbacks(&callbacks->gpu);
        ENGINE_PROFILE_END("GpuManagerRegisterCallbacks");

    OsManagerRegisterCallbacks(&callbacks->os);
        ENGINE_PROFILE_END("OsManagerRegisterCallbacks");
    OsManagerFinishLaunching();
        ENGINE_PROFILE_END("OsManagerFinishLaunching");

    WorldManagerLoadRandom(config->world_grid_random_count);
        ENGINE_PROFILE_END("WorldManagerLoadRandom");

        ENGINE_PROFILE_END("EngineProfile empty timing 1");
        ENGINE_PROFILE_END("EngineProfile empty timing 2");
}

void EngineManagerDestroy(void)
{
        ENGINE_PROFILE_START();
    WorldManagerDestroy();
        ENGINE_PROFILE_END("WorldManagerDestroy");
    CameraManagerDestroy();
        ENGINE_PROFILE_END("CameraManagerDestroy");
    RenderManagerDestroy();
        ENGINE_PROFILE_END("RenderManagerDestroy");
    OsManagerWindowDestroy();
        ENGINE_PROFILE_END("OsManagerWindowDestroy");
    GpuManagerDestroy();
        ENGINE_PROFILE_END("GpuManagerDestroy");
    OsManagerDestroy();
        ENGINE_PROFILE_END("OsManagerDestroy");
    log_manager_destroy();
        // No more logging allowed
}

void EngineManagerRun(void)
{
    OsManagerEventLoopRun();
}
