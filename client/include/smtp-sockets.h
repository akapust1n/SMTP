#ifndef __SMTP_SOCKETS_H__
#define __SMTP_SOCKETS_H__

void socket_set_nonblocking(int socket, bool is_nonblocking);
int create_local_socket_pair(int *out_sockets);
int socket_write(int socket, char *buf, int len);
int socket_read(int socket, char *buf, int len);

#endif
