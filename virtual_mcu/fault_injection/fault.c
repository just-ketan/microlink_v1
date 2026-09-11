#include "fault.h"

#include <string.h>

void ml_fault_cfg_clear(ml_fault_cfg_t *f)
{
    if (f != NULL) {
        memset(f, 0, sizeof(*f));
        f->stuck_gpio_bit = -1;
    }
}
