#include "grep.h"

#include <stdio.h>
#include <string.h>

#include "common.h"
#include "logger.h"
#include "matcher.h"

static void flush_long_line_remainder(FILE *fp) {
    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            break;
        }
    }
}

int run_grep(const CliOptions *opts, GrepStats *stats, ErrorCode *err) {
    FILE *fp;
    char line[MINI_GREP_MAX_LINE];
    size_t line_no = 0;

    if (opts == NULL || stats == NULL || err == NULL) {
        return -1;
    }

    memset(stats, 0, sizeof(*stats));
    *err = ERR_OK;

    fp = fopen(opts->file_path, "r");
    if (fp == NULL) {
        *err = ERR_FILE_OPEN;
        return -1;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        line_no++;
        stats->total_lines++;

        if (line_matches(line, opts->keyword, opts->ignore_case)) {
            stats->matched_lines++;
            if (opts->show_line_number) {
                printf("%zu:%s", line_no, line);
                if (line[0] != '\0' && line[strlen(line) - 1] != '\n') {
                    putchar('\n');
                }
            } else {
                fputs(line, stdout);
                if (line[0] != '\0' && line[strlen(line) - 1] != '\n') {
                    putchar('\n');
                }
            }
        }

        if (strchr(line, '\n') == NULL && !feof(fp)) {
            stats->truncated_lines++;
            log_error("line %zu exceeds %d chars, truncated", line_no, MINI_GREP_MAX_LINE - 1);
            flush_long_line_remainder(fp);
        }
    }

    if (ferror(fp)) {
        fclose(fp);
        *err = ERR_IO;
        return -1;
    }

    fclose(fp);
    return 0;
}
