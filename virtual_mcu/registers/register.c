#include "register.h"

#include <string.h>

static uint32_t load_le32(const uint8_t *p)
{
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static void store_le32(uint8_t *p, uint32_t v)
{
    p[0] = (uint8_t)(v & 0xFFu);
    p[1] = (uint8_t)((v >> 8) & 0xFFu);
    p[2] = (uint8_t)((v >> 16) & 0xFFu);
    p[3] = (uint8_t)((v >> 24) & 0xFFu);
}

ml_status_t ml_reg_bank_init(ml_reg_bank_t *bank,
                             uint32_t base,
                             uint32_t size,
                             uint8_t *storage,
                             const ml_reg_def_t *defs,
                             size_t ndefs,
                             void *owner,
                             ml_reg_on_write_fn on_write,
                             ml_reg_on_read_fn on_read)
{
    if (bank == NULL || storage == NULL || defs == NULL || size < 4u) {
        return ML_ERR_INVALID_ARG;
    }
    memset(bank, 0, sizeof(*bank));
    bank->base = base;
    bank->size = size;
    bank->storage = storage;
    bank->defs = defs;
    bank->ndefs = ndefs;
    bank->owner = owner;
    bank->on_write = on_write;
    bank->on_read = on_read;
    memset(storage, 0, size);
    ml_reg_bank_reset(bank);
    return ML_OK;
}

void ml_reg_bank_reset(ml_reg_bank_t *bank)
{
    if (bank == NULL) {
        return;
    }
    memset(bank->storage, 0, bank->size);
    for (size_t i = 0; i < bank->ndefs; i++) {
        const ml_reg_def_t *d = &bank->defs[i];
        if (d->offset + 4u <= bank->size) {
            store_le32(bank->storage + d->offset, d->reset_value);
        }
    }
}

ml_status_t ml_reg_lookup(const ml_reg_bank_t *bank, uint32_t offset, const ml_reg_def_t **out)
{
    if (bank == NULL || out == NULL) {
        return ML_ERR_INVALID_ARG;
    }
    for (size_t i = 0; i < bank->ndefs; i++) {
        if (bank->defs[i].offset == offset) {
            *out = &bank->defs[i];
            return ML_OK;
        }
    }
    return ML_ERR_NOT_FOUND;
}

uint32_t ml_reg_peek32(const ml_reg_bank_t *bank, uint32_t offset)
{
    if (bank == NULL || offset + 4u > bank->size) {
        return 0u;
    }
    return load_le32(bank->storage + offset);
}

void ml_reg_poke32(ml_reg_bank_t *bank, uint32_t offset, uint32_t value)
{
    if (bank == NULL || offset + 4u > bank->size) {
        return;
    }
    store_le32(bank->storage + offset, value);
}

static ml_status_t validate_abs(const ml_reg_bank_t *bank, uint32_t abs_addr, uint32_t *offset)
{
    if (bank == NULL) {
        return ML_ERR_INVALID_ARG;
    }
    if (abs_addr < bank->base) {
        return ML_ERR_INVALID_ADDR;
    }
    uint32_t off = abs_addr - bank->base;
    if (off >= bank->size) {
        return ML_ERR_INVALID_ADDR;
    }
    if ((abs_addr & 3u) != 0u) {
        return ML_ERR_UNALIGNED;
    }
    if (off + 4u > bank->size) {
        return ML_ERR_BOUNDARY;
    }
    *offset = off;
    return ML_OK;
}

ml_status_t ml_reg_read32(ml_reg_bank_t *bank, uint32_t abs_addr, uint32_t *out)
{
    uint32_t offset = 0;
    ml_status_t st;
    const ml_reg_def_t *def = NULL;

    if (out == NULL) {
        return ML_ERR_INVALID_ARG;
    }
    st = validate_abs(bank, abs_addr, &offset);
    if (st != ML_OK) {
        return st;
    }
    st = ml_reg_lookup(bank, offset, &def);
    if (st != ML_OK) {
        return ML_ERR_INVALID_ADDR;
    }
    if (def->access == ML_REG_WO) {
        return ML_ERR_ACCESS;
    }
    uint32_t value = load_le32(bank->storage + offset);
    if (bank->on_read != NULL) {
        bank->on_read(bank->owner, offset, &value);
    }
    value &= def->read_mask;
    *out = value;
    return ML_OK;
}

ml_status_t ml_reg_write32(ml_reg_bank_t *bank, uint32_t abs_addr, uint32_t value)
{
    uint32_t offset = 0;
    ml_status_t st;
    const ml_reg_def_t *def = NULL;

    st = validate_abs(bank, abs_addr, &offset);
    if (st != ML_OK) {
        return st;
    }
    st = ml_reg_lookup(bank, offset, &def);
    if (st != ML_OK) {
        return ML_ERR_INVALID_ADDR;
    }
    if (def->access == ML_REG_RO) {
        return ML_ERR_ACCESS;
    }
    uint32_t old = load_le32(bank->storage + offset);
    uint32_t written = (old & ~def->write_mask) | (value & def->write_mask);
    if (bank->stuck_mask != 0u) {
        written = (written & ~bank->stuck_mask) | (bank->stuck_value & bank->stuck_mask);
    }
    store_le32(bank->storage + offset, written);
    if (bank->on_write != NULL) {
        bank->on_write(bank->owner, offset, old, written);
    }
    return ML_OK;
}
