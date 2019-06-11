#include "log/log.h"
#include "test.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define LogMockBufferLength 4096

typedef struct {
    char *subsystem;
    char *category;
    i64 buffer_i;
    i64 buffer_i_start;
    char buffer[LogMockBufferLength];
} LogMock;

// Order of this has to match LogLevel enum in "os/log.h"
static const char *LogMockLevelStrings[] = {
    "[default]  ",
    "[info]  ",
    "[debug]  ",
    "[error]  ",
};

void log_mock_reset(Log *log)
{
    LogMock *mock = (LogMock *) log->log_impl;
    mock->buffer_i = mock->buffer_i_start;
    mock->buffer[mock->buffer_i_start] = '\0';
}

void log_init(Log *log, LogConfig *config)
{
    LogMock *mock = tmalloc(sizeof *mock);

    mock->subsystem = strcpy(mock->buffer, config->subsystem);
    mock->buffer_i_start = strlen(config->subsystem) + 1;
    mock->category = strcpy(&mock->buffer[mock->buffer_i_start],
            config->category);
    mock->buffer_i_start += strlen(config->category) + 1;

    log->log_impl = (void *) mock;

    log_mock_reset(log);
}

void log_destroy(Log *log)
{
    (void) log;
}

void log_at_level(Log *log, LogLevel level, const char *format, ...)
{
	va_list argptr;
	va_start(argptr, format);

    LogMock *mock = (LogMock *) log->log_impl;

    const char *level_string = LogMockLevelStrings[(i32) level];
    i32f level_string_length = (i32f) strlen(level_string);

	i64 size = LogMockBufferLength - mock->buffer_i;

	if (size > level_string_length) {
        for (i32f i = 0; i < level_string_length; i++) {
            mock->buffer[mock->buffer_i++] = level_string[i];
        }

        size -= level_string_length;
    }

    if (size > 0) {
		i64 output_length = vsnprintf(&mock->buffer[mock->buffer_i], size,
                format, argptr);
        if (output_length >= size) {
            output_length = size - 1;
        }

		mock->buffer_i += output_length;
		mock->buffer[mock->buffer_i++] = '\n';
		mock->buffer[mock->buffer_i] = '\0';
	}

    va_end(argptr);
}
