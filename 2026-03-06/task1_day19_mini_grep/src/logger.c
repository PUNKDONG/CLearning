#include "logger.h"

#include <stdio.h>
#include <time.h>

/* TODO: 你来实现可选的文件日志输出 */

int logger_init(const char *log_path) {
    (void)log_path;
    return 0;
}

void logger_close(void) {
}

void logger_log(LogLevel level, const char *fmt, ...) {
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    const char *lv = (level == LOG_LEVEL_ERROR) ? "ERROR" : "INFO";

    if (tm_now != NULL) {
        fprintf(stderr,
                "[%04d-%02d-%02d %02d:%02d:%02d] [%s] ",
                tm_now->tm_year + 1900,
                tm_now->tm_mon + 1,
                tm_now->tm_mday,
                tm_now->tm_hour,
                tm_now->tm_min,
                tm_now->tm_sec,
                lv);
    } else {
        fprintf(stderr, "[time-unknown] [%s] ", lv);
    }

    /* TODO: 可变参数输出到 stderr（以及可选的日志文件） */
    (void)fmt;
    fputc('\n', stderr);
}
