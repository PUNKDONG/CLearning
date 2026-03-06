#include "logger.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

static FILE *g_log_file = NULL;

static const char *level_to_text(LogLevel level) {
    return (level == LOG_LEVEL_ERROR) ? "ERROR" : "INFO";
}

static void format_time(char *buf, size_t cap) {
    time_t now = time(NULL);
    struct tm tm_now;
#if defined(_POSIX_THREAD_SAFE_FUNCTIONS)
    localtime_r(&now, &tm_now);
#else
    struct tm *tmp = localtime(&now);
    if (tmp != NULL) {
        tm_now = *tmp;
    } else {
        memset(&tm_now, 0, sizeof(tm_now));
    }
#endif
    strftime(buf, cap, "%Y-%m-%d %H:%M:%S", &tm_now);
}

int logger_init(const char *log_path) {
    if (log_path == NULL) {
        return -1;
    }
    g_log_file = fopen(log_path, "a");
    if (g_log_file == NULL) {
        return -1;
    }
    return 0;
}

void logger_close(void) {
    if (g_log_file != NULL) {
        fclose(g_log_file);
        g_log_file = NULL;
    }
}

void logger_log(LogLevel level, const char *fmt, ...) {
    char ts[32];
    va_list ap;

    if (fmt == NULL) {
        return;
    }

    format_time(ts, sizeof(ts));

    fprintf(stderr, "[%s] [%s] ", ts, level_to_text(level));
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputc('\n', stderr);

    if (g_log_file != NULL) {
        fprintf(g_log_file, "[%s] [%s] ", ts, level_to_text(level));
        va_start(ap, fmt);
        vfprintf(g_log_file, fmt, ap);
        va_end(ap);
        fputc('\n', g_log_file);
        fflush(g_log_file);
    }
}
