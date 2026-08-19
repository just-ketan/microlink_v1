#ifndef MICRLINK_APPLICATION_H
#define MICRLINK_APPLICATION_H

#include "microlink/sys.h"

#ifdef __cplusplus
extern "C" {
#endif

ml_status_t application_init(ml_sys_t *sys);
void application_step(ml_sys_t *sys);
void application_shutdown(ml_sys_t *sys);

#ifdef __cplusplus
}
#endif

#endif
