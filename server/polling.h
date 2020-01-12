#ifndef POLLING_H
#define POLLING_H
#include "logger.h"
#include "serveClient.h"
#include <map>
#include <string>
constexpr const int timeout = 3 * 60 * 1000; // 3min
#define MAX_CLIENTS 50
#define CLIENT_TIMEOUT 500

inline int doPoll(pollfd fds[100], int listenSocket, int& nfds, ClientsMap& clients)
{
    int newSocket = -1;
    bool closeConn = false;
    bool resizeArray = true;

    LOG_DEBUG("Waiting on poll() at thread=%lu \n", pthread_self());
    int numDescriptors = poll(fds, (nfds_t)nfds, timeout);

    if (numDescriptors < 0) {
        LOG_DEBUG("  poll() failed");
        return -1;
    }

    if (numDescriptors == 0) {
        LOG_DEBUG("  poll() timed out.  End program.\n");
        return -1;
    }
    while (true) {
        int currentSize = nfds;
        for (int i = 0; i < currentSize; ++i) {
            if (fds[i].revents == 0)
                continue;

            if (!(fds[i].revents & POLLIN)) {
                LOG_DEBUG("Error! revents=%d thread=%lu", fds[i].revents, pthread_self());
                return -1;
            }
            if (fds[i].fd == listenSocket) {
                LOG_DEBUG(" Listening socket is readable. Socket=%d thread=%lu", listenSocket, pthread_self());
                if (nfds >= MAX_CLIENTS) {
                    LOG_DEBUG("reach limit of client thread=%lu", pthread_self());
                    continue;
                }
                do {
                    newSocket = accept(listenSocket, nullptr, nullptr);
                    if (newSocket < 0) {
                        if (errno == EWOULDBLOCK) { //multithreading accept
                            break;
                        }
                    }
                    struct timeval sock_timeout;
                    sock_timeout.tv_sec = CLIENT_TIMEOUT;
                    sock_timeout.tv_usec = 0;
                    setsockopt(newSocket, SOL_SOCKET, SO_RCVTIMEO, &sock_timeout, sizeof(sock_timeout));

                    LOG_DEBUG("New incoming connection %d thread=%lu", newSocket, pthread_self());
                    fds[nfds].fd = newSocket;
                    fds[nfds].events = POLLIN;
                    ++nfds;
                    // init state TODO REPLACE ENUM VALUE
                    clients[newSocket] = Client(newSocket, serverBufferSize, -1, 1);

                } while (newSocket != -1);
            }

            else {
                // todo ??
            }
            // serve sockets
        }
    }
}
#endif // POLLING_H
