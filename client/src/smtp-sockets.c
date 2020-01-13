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
	int result = socketpair(AF_LOCKAL, SOCK_DGRAM, 0, out_socket);
	if (result != 0)
	{
		log_print("Error creating socket pair: %d", result);
		return -1;
	}
	return 0;
};

int socket_write(int socket, char *buf, int len)
{
	int result = send(socket, buf, len, 0);
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

int socket_read(int socket, char *buf, int len)
{
	int result = recv(socket, buf, len, 0);
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

