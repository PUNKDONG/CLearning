#include "error.h"

const char *error_code_to_string(ErrorCode code) {
    switch (code) {
    case ERR_OK:
        return "ok";
    case ERR_INVALID_ARGS:
        return "invalid arguments";
    case ERR_FILE_OPEN:
        return "failed to open file";
    case ERR_IO:
        return "file I/O error";
    case ERR_NOMEM:
        return "out of memory";
    default:
        return "unknown error";
    }
}
