#include "grep.h"

#include <stdio.h>
#include <string.h>

#include "common.h"
#include "logger.h"
#include "matcher.h"

int run_grep(const CliOptions *opts, GrepStats *stats, ErrorCode *err) {
    FILE *fp;
    char line[MINI_GREP_MAX_LINE];
    size_t line_no = 0;

    if (opts == NULL || stats == NULL || err == NULL) {
        return -1;
    }

    stats->total_lines = 0;
    stats->matched_lines = 0;
    stats->truncated_lines = 0;
    *err = ERR_OK;

    fp = fopen(opts->file_path, "r");
    if (fp == NULL) {
        *err = ERR_FILE_OPEN;
        return -1;
    }

    /* TODO:
     * 1) 循环读行
     * 2) 维护 total_lines/line_no
     * 3) 调 line_matches
     * 4) 根据 show_line_number 决定输出格式
     * 5) 处理超长行（truncated_lines + 跳过剩余字符）
     */
    while (fgets(line, sizeof(line), fp) != NULL) {
        (void)line_no;
    }

    if (ferror(fp)) {
        fclose(fp);
        *err = ERR_IO;
        return -1;
    }

    fclose(fp);
    return 0;
}
