#ifndef MICRLINK_STATUS_H
#define MICRLINK_STATUS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ml_status {
    ML_OK = 0,
    ML_ERR_INVALID_ARG = 1,
    ML_ERR_INVALID_ADDR = 2,
    ML_ERR_UNALIGNED = 3,
    ML_ERR_BOUNDARY = 4,
    ML_ERR_ACCESS = 5,
    ML_ERR_NOT_FOUND = 6,
    ML_ERR_BUSY = 7,
    ML_ERR_TIMEOUT = 8,
    ML_ERR_NOMEM = 9,
    ML_ERR_IO = 10,
    ML_ERR_PROTOCOL = 11,
    ML_ERR_UNSUPPORTED = 12,
    ML_ERR_FAULT = 13,
    ML_ERR_NACK = 14,
    ML_ERR_EMPTY = 15,
    ML_ERR_FULL = 16,
    ML_ERR_STATE = 17
} ml_status_t;

static inline const char *ml_status_str(ml_status_t st)
{
    switch (st) {
    case ML_OK:
        return "ok";
    case ML_ERR_INVALID_ARG:
        return "invalid argument";
    case ML_ERR_INVALID_ADDR:
        return "invalid address";
    case ML_ERR_UNALIGNED:
        return "unaligned access";
    case ML_ERR_BOUNDARY:
        return "access crosses boundary";
    case ML_ERR_ACCESS:
        return "access denied";
    case ML_ERR_NOT_FOUND:
        return "not found";
    case ML_ERR_BUSY:
        return "busy";
    case ML_ERR_TIMEOUT:
        return "timeout";
    case ML_ERR_NOMEM:
        return "out of memory";
    case ML_ERR_IO:
        return "io error";
    case ML_ERR_PROTOCOL:
        return "protocol error";
    case ML_ERR_UNSUPPORTED:
        return "unsupported";
    case ML_ERR_FAULT:
        return "injected fault";
    case ML_ERR_NACK:
        return "nack";
    case ML_ERR_EMPTY:
        return "empty";
    case ML_ERR_FULL:
        return "full";
    case ML_ERR_STATE:
        return "invalid state";
    default:
        return "unknown";
    }
}

#ifdef __cplusplus
}
#endif

#endif /* MICRLINK_STATUS_H */
