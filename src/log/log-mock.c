#include "log/log.h"
#include "test/allocator.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define LogMockBufferLength 4096

struct Log {
    char *subsystem;
    char *category;
    i64 buffer_i;
    i64 buffer_i_start;
    char buffer[LogMockBufferLength];
};

// Order of this has to match LogLevel enum in "log/log.h"
static const char *LogMockLevelStrings[] = {
    "[default]  ",
    "[info]  ",
    "[debug]  ",
    "[error]  ",
};

void log_mock_reset(Log *log)
{
    log->buffer_i = log->buffer_i_start;
    log->buffer[log->buffer_i_start] = '\0';
}

Log *log_create(LogConfig *config)
{
    Log *log = tmalloc(sizeof *log);

    log->subsystem = strcpy(log->buffer, config->subsystem);
    log->buffer_i_start = strlen(config->subsystem) + 1;
    log->category = strcpy(&log->buffer[log->buffer_i_start],
            config->category);
    log->buffer_i_start += strlen(config->category) + 1;

    log_mock_reset(log);

    return log;
}

void log_destroy(Log *log)
{
    tfree(log);
}

void log_at_level(Log *log, LogLevel level, const char *format, ...)
{
	va_list argptr;
	va_start(argptr, format);

    const char *level_string = LogMockLevelStrings[(i32) level];
    i32f level_string_length = (i32f) strlen(level_string);

	i64 size = LogMockBufferLength - log->buffer_i;

	if (size > level_string_length) {
        for (i32f i = 0; i < level_string_length; i++) {
            log->buffer[log->buffer_i++] = level_string[i];
        }

        size -= level_string_length;
    }

    if (size > 0) {
		i64 output_length = vsnprintf(&log->buffer[log->buffer_i], size,
                format, argptr);
        if (output_length >= size) {
            output_length = size - 1;
        }

		log->buffer_i += output_length;
		log->buffer[log->buffer_i++] = '\n';
		log->buffer[log->buffer_i] = '\0';
	}

    va_end(argptr);
}
