#include "memory_map.h"

ml_status_t ml_memory_map_init(ml_memory_map_t *map)
{
    if (map == NULL) {
        return ML_ERR_INVALID_ARG;
    }
    map->nregions = 0;
    for (size_t i = 0; i < ML_MAP_MAX_REGIONS; i++) {
        map->regions[i] = NULL;
    }
    return ML_OK;
}

ml_status_t ml_memory_map_add(ml_memory_map_t *map, ml_reg_bank_t *bank)
{
    if (map == NULL || bank == NULL) {
        return ML_ERR_INVALID_ARG;
    }
    if (map->nregions >= ML_MAP_MAX_REGIONS) {
        return ML_ERR_NOMEM;
    }
    map->regions[map->nregions++] = bank;
    return ML_OK;
}

ml_status_t ml_memory_map_lookup(ml_memory_map_t *map, uint32_t abs_addr, ml_reg_bank_t **out)
{
    if (map == NULL || out == NULL) {
        return ML_ERR_INVALID_ARG;
    }
    for (size_t i = 0; i < map->nregions; i++) {
        ml_reg_bank_t *b = map->regions[i];
        if (abs_addr >= b->base && abs_addr < b->base + b->size) {
            *out = b;
            return ML_OK;
        }
    }
    return ML_ERR_INVALID_ADDR;
}

ml_status_t ml_memory_map_read32(ml_memory_map_t *map, uint32_t abs_addr, uint32_t *out)
{
    ml_reg_bank_t *bank = NULL;
    ml_status_t st = ml_memory_map_lookup(map, abs_addr, &bank);
    if (st != ML_OK) {
        return st;
    }
    return ml_reg_read32(bank, abs_addr, out);
}

ml_status_t ml_memory_map_write32(ml_memory_map_t *map, uint32_t abs_addr, uint32_t value)
{
    ml_reg_bank_t *bank = NULL;
    ml_status_t st = ml_memory_map_lookup(map, abs_addr, &bank);
    if (st != ML_OK) {
        return st;
    }
    return ml_reg_write32(bank, abs_addr, value);
}
