#include "engine/manager.h"
#include "render/layout.h"

EngineConfig engine_config = {
    .log = {
        .logs = {
            .log = {
                .subsystem = "computer.spacetime.log",
                .category = "default",
            },
            .os = {
                .subsystem = "computer.spacetime.os",
                .category = "default",
            },
            .gpu = {
                .subsystem = "computer.spacetime.gpu",
                .category = "default",
            },
            .string = {
                .subsystem = "computer.spacetime.string",
                .category = "default",
            },
            .resource = {
                .subsystem = "computer.spacetime.resource",
                .category = "default",
            },
            .render = {
                .subsystem = "computer.spacetime.render",
                .category = "default",
            },
            .camera = {
                .subsystem = "computer.spacetime.camera",
                .category = "default",
            },
            .world = {
                .subsystem = "computer.spacetime.world",
                .category = "default",
            },
        },
    },
    .os = {
        .application = {
            .will_terminate = engine_manager_will_terminate,
        },
        .event_queue_length = 64,
        .event_queue_safe_length_remaining = 16,
    },
    .gpu = {
        .view = {
            .draw_in_view = gpu_manager_draw_in_view,  // TODO: render_manager_draw_in_view
            .size_changed = render_manager_size_changed,
            .preferred_frames_per_second = 60,
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

void engine_manager_init(EngineConfig *config)
{
    log_manager_init(&config->log);
    os_manager_init(&config->os);
    gpu_manager_init(&config->gpu);
    string_manager_init(&config->string);
    resource_manager_init(&config->resource);

    os_manager_window_set_view(&gGpuManager.view);
    config->render.viewport_size = gGpuManager.view.viewport_size;

    render_manager_init(&config->render);
    camera_manager_init(&config->camera);
    world_manager_init(&config->world);

    os_manager_finish_launching();

    world_manager_load_random(config->world_grid_random_count);
}

void engine_manager_destroy(void)
{
    world_manager_destroy();
    camera_manager_destroy();
    render_manager_destroy();
    resource_manager_destroy();
    string_manager_destroy();
    gpu_manager_destroy();
    os_manager_destroy();
    log_manager_destroy();
}

static void engine_manager_will_terminate(OsNotification *notification)
{
    (void)notification;
    engine_manager_destroy();
}

void engine_manager_run(void)
{
    os_manager_run_event_loop();
}
