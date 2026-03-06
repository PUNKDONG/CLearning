#include "matcher.h"

#include <ctype.h>
#include <stddef.h>
#include <string.h>

static int contains_case_sensitive(const char *line, const char *keyword) {
    return strstr(line, keyword) != NULL;
}

static int contains_case_insensitive(const char *line, const char *keyword) {
    size_t i;
    size_t j;
    size_t key_len;

    key_len = strlen(keyword);
    if (key_len == 0) {
        return 1;
    }

    for (i = 0; line[i] != '\0'; i++) {
        for (j = 0; j < key_len; j++) {
            unsigned char c1 = (unsigned char)line[i + j];
            unsigned char c2 = (unsigned char)keyword[j];
            if (line[i + j] == '\0') {
                return 0;
            }
            if (tolower(c1) != tolower(c2)) {
                break;
            }
        }
        if (j == key_len) {
            return 1;
        }
    }

    return 0;
}

int line_matches(const char *line, const char *keyword, int ignore_case) {
    if (line == NULL || keyword == NULL) {
        return 0;
    }
    if (ignore_case) {
        return contains_case_insensitive(line, keyword);
    }
    return contains_case_sensitive(line, keyword);
}
