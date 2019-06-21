#include "Log/Manager.c"
#include "Log/LogMock.c"

LogManagerConfig gLogManagerMockConfig = {
    .logs = {
        .os = {
            .subsystem = "computer.spacetime-test.os",
            .category = "default",
        },
        .gpu = {
            .subsystem = "computer.spacetime-test.gpu",
            .category = "default",
        },
        .engine = {
            .subsystem = "computer.spacetime-test.engine",
            .category = "default",
        },
    },
};
