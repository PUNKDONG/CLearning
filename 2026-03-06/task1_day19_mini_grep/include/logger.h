#ifndef LOGGER_H
#define LOGGER_H

#include <stdarg.h>

typedef enum {
    LOG_LEVEL_INFO = 0,
    LOG_LEVEL_ERROR = 1
} LogLevel;

int logger_init(const char *log_path);
void logger_close(void);
void logger_log(LogLevel level, const char *fmt, ...);

#define log_info(...) logger_log(LOG_LEVEL_INFO, __VA_ARGS__)
#define log_error(...) logger_log(LOG_LEVEL_ERROR, __VA_ARGS__)

#endif
