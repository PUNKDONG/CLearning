#include <stdio.h>
#include <string.h>

#include "args.h"

static void print_usage(const char *prog) {
    printf("Usage: %s [-i] [-n] <word> <filepath>\n", prog);
}

int parse_args(int argc, char **argv, Arg *args) {
    int positional_count = 0;
    const char *word = NULL;
    const char *filepath = NULL;

    if (args == NULL || argv == NULL || argc <= 0) {
        return 1;
    }

    args->i_flag = 0;
    args->n_flag = 0;
    args->word = NULL;
    args->filepath = NULL;

    for (int i = 1; i < argc; i++) {
        const char *token = argv[i];

        if (strcmp(token, "-i") == 0) {
            args->i_flag = 1;
            continue;
        }
        if (strcmp(token, "-n") == 0) {
            args->n_flag = 1;
            continue;
        }
        if (token[0] == '-') {
            print_usage(argv[0]);
            return 1;
        }

        if (positional_count == 0) {
            word = token;
            positional_count++;
        } else if (positional_count == 1) {
            filepath = token;
            positional_count++;
        } else {
            print_usage(argv[0]);
            return 1;
        }
    }

    if (positional_count != 2) {
        print_usage(argv[0]);
        return 1;
    }

 
        FILE *fp = fopen(filepath, "r");
        if (fp == NULL) {
            perror(filepath);
            return 1;
        }
        fclose(fp);
  

    args->word = word;
    args->filepath = filepath;
    return 0;
}
