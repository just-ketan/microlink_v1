#ifndef MICRLINK_REGISTER_H
#define MICRLINK_REGISTER_H

#include "microlink/status.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ml_reg_access {
    ML_REG_RO = 0,
    ML_REG_WO = 1,
    ML_REG_RW = 2
} ml_reg_access_t;

typedef struct ml_reg_def {
    uint32_t offset;
    uint32_t reset_value;
    uint32_t read_mask;
    uint32_t write_mask;
    ml_reg_access_t access;
    const char *name;
} ml_reg_def_t;

typedef struct ml_reg_bank ml_reg_bank_t;

typedef void (*ml_reg_on_write_fn)(void *owner, uint32_t offset, uint32_t old_value, uint32_t new_value);
typedef void (*ml_reg_on_read_fn)(void *owner, uint32_t offset, uint32_t *value);

struct ml_reg_bank {
    uint32_t base;
    uint32_t size;
    uint8_t *storage;
    const ml_reg_def_t *defs;
    size_t ndefs;
    void *owner;
    ml_reg_on_write_fn on_write;
    ml_reg_on_read_fn on_read;
    uint32_t stuck_mask;
    uint32_t stuck_value;
};

ml_status_t ml_reg_bank_init(ml_reg_bank_t *bank,
                             uint32_t base,
                             uint32_t size,
                             uint8_t *storage,
                             const ml_reg_def_t *defs,
                             size_t ndefs,
                             void *owner,
                             ml_reg_on_write_fn on_write,
                             ml_reg_on_read_fn on_read);

void ml_reg_bank_reset(ml_reg_bank_t *bank);

ml_status_t ml_reg_lookup(const ml_reg_bank_t *bank, uint32_t offset, const ml_reg_def_t **out);

ml_status_t ml_reg_read32(ml_reg_bank_t *bank, uint32_t abs_addr, uint32_t *out);
ml_status_t ml_reg_write32(ml_reg_bank_t *bank, uint32_t abs_addr, uint32_t value);

uint32_t ml_reg_peek32(const ml_reg_bank_t *bank, uint32_t offset);
void ml_reg_poke32(ml_reg_bank_t *bank, uint32_t offset, uint32_t value);

#ifdef __cplusplus
}
#endif

#endif
