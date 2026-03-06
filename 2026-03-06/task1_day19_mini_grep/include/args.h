#ifndef ARGS_H
#define ARGS_H

#include "error.h"

typedef struct {
    int ignore_case;
    int show_line_number;
    const char *keyword;
    const char *file_path;
} CliOptions;

int parse_args(int argc, char **argv, CliOptions *opts, ErrorCode *err, const char **err_msg);
void print_usage(const char *prog);

#endif
