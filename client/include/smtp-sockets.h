#ifndef __SMTP_SOCKETS_H__
#define __SMTP_SOCKETS_H__

#include <stdbool.h>
#include <sys/socket.h>

void socket_set_nonblocking(int socket, bool is_nonblocking);
int create_local_socket_pair(int *out_sockets);
ssize_t socket_write(int socket, char *buf, int len);
ssize_t socket_read(int socket, char *buf, int len);

#endif
