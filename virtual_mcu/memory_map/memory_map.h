#ifndef MICRLINK_MEMORY_MAP_H
#define MICRLINK_MEMORY_MAP_H

#include "../registers/register.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ML_MAP_MAX_REGIONS 8

typedef struct ml_memory_map {
    ml_reg_bank_t *regions[ML_MAP_MAX_REGIONS];
    size_t nregions;
} ml_memory_map_t;

ml_status_t ml_memory_map_init(ml_memory_map_t *map);
ml_status_t ml_memory_map_add(ml_memory_map_t *map, ml_reg_bank_t *bank);
ml_status_t ml_memory_map_lookup(ml_memory_map_t *map, uint32_t abs_addr, ml_reg_bank_t **out);
ml_status_t ml_memory_map_read32(ml_memory_map_t *map, uint32_t abs_addr, uint32_t *out);
ml_status_t ml_memory_map_write32(ml_memory_map_t *map, uint32_t abs_addr, uint32_t value);

#ifdef __cplusplus
}
#endif

#endif
