#ifndef MICRLINK_TCP_IPC_H
#define MICRLINK_TCP_IPC_H

#include "microlink/status.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ml_tcp {
    int fd;
    int connected;
} ml_tcp_t;

ml_status_t ml_tcp_connect(ml_tcp_t *t, const char *host, uint16_t port);
ml_status_t ml_tcp_send_line(ml_tcp_t *t, const char *line);
ml_status_t ml_tcp_recv_line(ml_tcp_t *t, char *buf, size_t cap);
void ml_tcp_close(ml_tcp_t *t);

#ifdef __cplusplus
}
#endif

#endif
