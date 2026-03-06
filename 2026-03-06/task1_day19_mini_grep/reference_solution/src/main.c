#include <stdio.h>

#include "args.h"
#include "error.h"
#include "grep.h"
#include "logger.h"

int main(int argc, char **argv) {
    CliOptions opts;
    GrepStats stats;
    ErrorCode err = ERR_OK;
    const char *arg_err_msg = "";

    if (logger_init("logs/mini_grep.log") != 0) {
        fprintf(stderr, "[WARN] cannot open logs/mini_grep.log, continue without file log\n");
    }

    if (parse_args(argc, argv, &opts, &err, &arg_err_msg) != 0) {
        log_error("arg parse failed: %s (%s)", arg_err_msg, error_code_to_string(err));
        print_usage(argv[0]);
        logger_close();
        return err;
    }

    log_info("start: keyword='%s' file='%s' ignore_case=%d show_line_number=%d",
             opts.keyword,
             opts.file_path,
             opts.ignore_case,
             opts.show_line_number);

    if (run_grep(&opts, &stats, &err) != 0) {
        log_error("run_grep failed: %s", error_code_to_string(err));
        logger_close();
        return err;
    }

    log_info("done: total_lines=%zu matched_lines=%zu truncated_lines=%zu",
             stats.total_lines,
             stats.matched_lines,
             stats.truncated_lines);

    logger_close();
    return ERR_OK;
}
