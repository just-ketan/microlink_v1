#ifndef MICRLINK_FW_MEM_H
#define MICRLINK_FW_MEM_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Firmware-side helper: validate logical MMIO ranges before driver access. */
int ml_fw_addr_in_mmio(uint32_t addr, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif
