#ifndef MICRLINK_JSONISH_H
#define MICRLINK_JSONISH_H

#include "microlink/status.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int ml_json_get_u32(const char *json, const char *key, uint32_t *out);
int ml_json_get_int(const char *json, const char *key, int *out);

#ifdef __cplusplus
}
#endif

#endif
