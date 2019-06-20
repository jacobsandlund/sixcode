#include "engine/manager.h"
#include "engine/profile.h"
#include "os/screen.h"
#include "render/layout.h"

EngineCallbacks gEngineCallbacks = {
    .os = {
        .application = {
            .will_terminate = engine_manager_destroy,
        },
    },
    .gpu = {
        .view = {
            .draw_in_view = gpu_manager_draw_in_view,
            .size_changed = gpu_manager_size_changed,
        },
    },
};

EngineConfig gEngineConfig = {
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
#ifdef __APPLE__
            .color_pixel_format = MTLPixelFormatBGRA8Unorm_sRGB,
#endif
        },
    },
    .string = {
        .string_table_size = 256,
    },
    .resource = {
        .loader_capacity = 64,
        .resource_capacity = 128,
        .descriptor_capacity = 256,
        .pointer_allocator_capacity = 4096,
    },
    .render = {
        .layout_type = RenderLayoutTypeHex,
        .viewport_size = {0.0, 0.0},        // Set after GpuView initialization
    },
    .camera = {
        .position = {0.0, 0.0, 32.0},
    },
    .world = {
        .grid_size = 4096,
    },
    .world_grid_random_count = 10000000,
};

void engine_manager_init(EngineCallbacks *callbacks, EngineConfig *config)
{
        EngineProfileStart();
    log_manager_init(&config->log);
        EngineProfileEnd("log_manager_init");
    os_manager_init(&config->os);
        EngineProfileEnd("os_manager_init");

    gpu_manager_init(os_screen_visible_frame(), &config->gpu);
        EngineProfileEnd("gpu_manager_init");
    string_manager_init(&config->string);
        EngineProfileEnd("string_manager_init");
    resource_manager_init(&config->resource);
        EngineProfileEnd("resource_manager_init");

    os_manager_window_init(gGpuManager.view);
        EngineProfileEnd("os_manager_window_init");

    render_manager_init(&config->render);
        EngineProfileEnd("render_manager_init");
    camera_manager_init(&config->camera);
        EngineProfileEnd("camera_manager_init");
    world_manager_init(&config->world);
        EngineProfileEnd("camera_manager_init");
    gpu_manager_register_callbacks(&callbacks->gpu);
        EngineProfileEnd("gpu_manager_register_callbacks");

    os_manager_register_callbacks(&callbacks->os);
        EngineProfileEnd("os_manager_register_callbacks");
    os_manager_finish_launching();
        EngineProfileEnd("os_manager_finish_launching");

    world_manager_load_random(config->world_grid_random_count);
        EngineProfileEnd("world_manager_load_random");

        EngineProfileEnd("EngineProfile empty timing 1");
        EngineProfileEnd("EngineProfile empty timing 2");
}

void engine_manager_destroy(void)
{
        EngineProfileStart();
    world_manager_destroy();
        EngineProfileEnd("world_manager_destroy");
    camera_manager_destroy();
        EngineProfileEnd("camera_manager_destroy");
    render_manager_destroy();
        EngineProfileEnd("render_manager_destroy");
    os_manager_window_destroy();
        EngineProfileEnd("os_manager_window_destroy");
    resource_manager_destroy();
        EngineProfileEnd("resource_manager_destroy");
    string_manager_destroy();
        EngineProfileEnd("string_manager_destroy");
    gpu_manager_destroy();
        EngineProfileEnd("gpu_manager_destroy");
    os_manager_destroy();
        EngineProfileEnd("os_manager_destroy");
    log_manager_destroy();
        // No more logging allowed
}

void engine_manager_run(void)
{
    os_manager_event_loop_run();
}
