#include "args.h"

#include <stdio.h>
#include <string.h>

void print_usage(const char *prog) {
    printf("Usage: %s [-i] [-n] <keyword> <file>\n", prog);
    printf("  -i    ignore case\n");
    printf("  -n    show line number\n");
}

int parse_args(int argc, char **argv, CliOptions *opts, ErrorCode *err, const char **err_msg) {
    int i = 1;

    if (opts == NULL || err == NULL || err_msg == NULL) {
        return -1;
    }

    memset(opts, 0, sizeof(*opts));
    *err = ERR_OK;
    *err_msg = "";

    while (i < argc && argv[i][0] == '-') {
        if (strcmp(argv[i], "-i") == 0) {
            opts->ignore_case = 1;
        } else if (strcmp(argv[i], "-n") == 0) {
            opts->show_line_number = 1;
        } else {
            *err = ERR_INVALID_ARGS;
            *err_msg = "unknown option";
            return -1;
        }
        i++;
    }

    if (argc - i != 2) {
        *err = ERR_INVALID_ARGS;
        *err_msg = "missing or extra positional arguments";
        return -1;
    }

    opts->keyword = argv[i];
    opts->file_path = argv[i + 1];

    if (opts->keyword[0] == '\0') {
        *err = ERR_INVALID_ARGS;
        *err_msg = "keyword must not be empty";
        return -1;
    }

    return 0;
}
