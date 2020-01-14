#include <fcntl.h>
#include <client-logger.h>
#include <errno.h>
#include <stdio.h>
#include "smtp_sockets.h"


void socket_set_nonblocking(int socket, bool is_nonblocking)
{
	int flags = fcntl(socket, F_GETFL);
	if (is_nonblocking)
	{
		fcntl(socket, F_SETFL, flags | O_NONBLOCK);
	}
	else
	{
		fcntl(socket, F_SETFL, flags & ~O_NONBLOCK);
	}
};

int create_local_socket_pair(int *out_sockets)
{
	int result = socketpair(AF_LOCAL, SOCK_DGRAM, 0, out_sockets);
	if (result != 0)
	{
		perror("Error creating socket pair");
		return -1;
	}
	return 0;
};

ssize_t socket_write(int socket, char *buf, int len)
{
	ssize_t result = send(socket, buf, len, 0);
	if (result >= 0)
	{
		return result;
	}
	if ((errno = EAGAIN) || (errno == EWOULDBLOCK))
	{
		return 0;
	}
	return -errno;
};

ssize_t socket_read(int socket, char *buf, int len)
{
	ssize_t result = recv(socket, buf, len, 0);
	if (result >= 0)
	{
		return result;
	}
	if ((errno = EAGAIN) || (errno == EWOULDBLOCK))
	{
		return 0;
	}
	return -errno;
};

