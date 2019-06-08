#include "log/manager.c"
#include "log/log-mock.c"

LogManagerConfig gLogManagerMockConfig = {
    .logs = {
        .log = {
            .subsystem = "computer.spacetime-test.log",
            .category = "default",
        },
        .os = {
            .subsystem = "computer.spacetime-test.os",
            .category = "default",
        },
        .gpu = {
            .subsystem = "computer.spacetime-test.gpu",
            .category = "default",
        },
        .string = {
            .subsystem = "computer.spacetime-test.string",
            .category = "default",
        },
        .resource = {
            .subsystem = "computer.spacetime-test.resource",
            .category = "default",
        },
        .render = {
            .subsystem = "computer.spacetime-test.render",
            .category = "default",
        },
        .camera = {
            .subsystem = "computer.spacetime-test.camera",
            .category = "default",
        },
        .world = {
            .subsystem = "computer.spacetime-test.world",
            .category = "default",
        },
    },
};
