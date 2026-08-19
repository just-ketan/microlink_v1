#include "tcp_ipc.h"

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
typedef SOCKET ml_sock;
#define ML_INVALID INVALID_SOCKET
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
typedef int ml_sock;
#define ML_INVALID (-1)
#endif

#ifdef _WIN32
static int wsa_ready = 0;
static ml_status_t ensure_wsa(void)
{
    WSADATA wsa;
    if (wsa_ready) {
        return ML_OK;
    }
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        return ML_ERR_IO;
    }
    wsa_ready = 1;
    return ML_OK;
}
#endif

ml_status_t ml_tcp_connect(ml_tcp_t *t, const char *host, uint16_t port)
{
    struct sockaddr_in addr;
    ml_sock s;
    if (t == NULL || host == NULL) {
        return ML_ERR_INVALID_ARG;
    }
    t->fd = -1;
    t->connected = 0;
#ifdef _WIN32
    if (ensure_wsa() != ML_OK) {
        return ML_ERR_IO;
    }
#endif
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == ML_INVALID) {
        return ML_ERR_IO;
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, host, &addr.sin_addr) != 1) {
#ifdef _WIN32
        closesocket(s);
#else
        close(s);
#endif
        return ML_ERR_INVALID_ARG;
    }
    if (connect(s, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
#ifdef _WIN32
        closesocket(s);
#else
        close(s);
#endif
        return ML_ERR_IO;
    }
    t->fd = (int)s;
    t->connected = 1;
    return ML_OK;
}

ml_status_t ml_tcp_send_line(ml_tcp_t *t, const char *line)
{
    size_t n;
    char buf[1024];
    if (t == NULL || !t->connected || line == NULL) {
        return ML_ERR_IO;
    }
    n = (size_t)snprintf(buf, sizeof(buf), "%s\n", line);
    if (n >= sizeof(buf)) {
        return ML_ERR_NOMEM;
    }
#ifdef _WIN32
    if (send((SOCKET)t->fd, buf, (int)n, 0) != (int)n) {
        return ML_ERR_IO;
    }
#else
    if (send(t->fd, buf, n, 0) != (ssize_t)n) {
        return ML_ERR_IO;
    }
#endif
    return ML_OK;
}

ml_status_t ml_tcp_recv_line(ml_tcp_t *t, char *buf, size_t cap)
{
    size_t i = 0;
    if (t == NULL || !t->connected || buf == NULL || cap < 2u) {
        return ML_ERR_IO;
    }
    while (i + 1u < cap) {
        char c = 0;
#ifdef _WIN32
        int r = recv((SOCKET)t->fd, &c, 1, 0);
#else
        ssize_t r = recv(t->fd, &c, 1, 0);
#endif
        if (r <= 0) {
            return ML_ERR_IO;
        }
        if (c == '\n') {
            break;
        }
        buf[i++] = c;
    }
    buf[i] = '\0';
    return ML_OK;
}

void ml_tcp_close(ml_tcp_t *t)
{
    if (t == NULL || !t->connected) {
        return;
    }
#ifdef _WIN32
    closesocket((SOCKET)t->fd);
#else
    close(t->fd);
#endif
    t->connected = 0;
    t->fd = -1;
}
