#ifndef GREP_H
#define GREP_H

#include <stddef.h>

#include "args.h"
#include "error.h"

typedef struct {
    size_t total_lines;
    size_t matched_lines;
    size_t truncated_lines;
} GrepStats;

int run_grep(const CliOptions *opts, GrepStats *stats, ErrorCode *err);

#endif
