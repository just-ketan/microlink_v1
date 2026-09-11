#include "jsonish.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *find_key(const char *json, const char *key)
{
    char pat[64];
    (void)snprintf(pat, sizeof(pat), "\"%s\"", key);
    const char *p = strstr(json, pat);
    if (p == NULL) {
        return NULL;
    }
    p = strchr(p, ':');
    if (p == NULL) {
        return NULL;
    }
    return p + 1;
}

int ml_json_get_u32(const char *json, const char *key, uint32_t *out)
{
    const char *p;
    char *end = NULL;
    unsigned long v;
    if (json == NULL || key == NULL || out == NULL) {
        return 0;
    }
    p = find_key(json, key);
    if (p == NULL) {
        return 0;
    }
    v = strtoul(p, &end, 10);
    if (end == p) {
        return 0;
    }
    *out = (uint32_t)v;
    return 1;
}

int ml_json_get_int(const char *json, const char *key, int *out)
{
    uint32_t v = 0;
    if (!ml_json_get_u32(json, key, &v)) {
        return 0;
    }
    *out = (int)v;
    return 1;
}
