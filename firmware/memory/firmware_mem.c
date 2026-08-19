#include "firmware_mem.h"

#include "microlink/types.h"

int ml_fw_addr_in_mmio(uint32_t addr, uint32_t size)
{
    if (size == 0u) {
        return 0;
    }
    if (addr < ML_GPIO_BASE) {
        return 0;
    }
    uint32_t end = ML_RADIO_BASE + ML_REGION_SIZE;
    if (addr >= end) {
        return 0;
    }
    if (size > end - addr) {
        return 0;
    }
    return 1;
}
