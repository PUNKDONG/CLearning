#ifndef ERROR_H
#define ERROR_H

typedef enum {
    ERR_OK = 0,
    ERR_INVALID_ARGS = 2,
    ERR_FILE_OPEN = 3,
    ERR_IO = 4,
    ERR_NOMEM = 5
} ErrorCode;

const char *error_code_to_string(ErrorCode code);

#endif
