#import "log/log.h"

@import AppKit;

#import <stdarg.h>
#import <stdio.h>
#import <os/log.h>

#define MacLogBufferLength 4096

// Order of this has to match LogLevel enum in "os/log.h"
static const i32 MacLogTypeLookup[] = {
    OS_LOG_TYPE_DEFAULT,
    OS_LOG_TYPE_INFO,
    OS_LOG_TYPE_DEBUG,
    OS_LOG_TYPE_ERROR,
};

void log_init(Log *log, LogConfig *config)
{
    os_log_t os_log = os_log_create(config->subsystem, config->category);
    log->log_impl = (void *) CFBridgingRetain(os_log);
}

void log_destroy(Log *log)
{
    CFRelease(log->log_impl);
}

void log_at_level(Log *log, LogLevel level, const char *format, ...)
{
    static char log_buffer[MacLogBufferLength];
	va_list argptr;
	va_start(argptr, format);

    os_log_t os_log = (__bridge os_log_t) log->log_impl;
    i32 log_type = MacLogTypeLookup[(i32) level];
    vsnprintf(log_buffer, MacLogBufferLength, format, argptr);
    os_log_with_type(os_log, log_type, "%{public}s", log_buffer);

    va_end(argptr);
}
