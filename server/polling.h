#ifndef POLLING_H
#define POLLING_H
#include "logger.h"
#include <map>
#include <string>

constexpr const int timeout = 3 * 60 * 1000; // 3min
#define MAX_CLIENTS 50
#define CLIENT_TIMEOUT 500
struct Client {
    std::string srv_curmsg;
    // te_ss_state srv_state;
    FILE** srv_output_files;
    char** srv_output_file_names;
    int srv_output_files_cnt;
};

using ClientsMap = std::map<int, Client>;

inline int doPoll(pollfd fds[100], int listenSocket, int& nfds, ClientsMap& clients)
{
    char buffer[100];

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

            } while (newSocket != -1);
        }

        else {
            LOG_DEBUG("Descriptor %d is readable thread=%lu", fds[i].fd, pthread_self());
            closeConn = false;

            do {
                int len = (int)recv(fds[i].fd, buffer, sizeof(buffer), 0);
                if (len < 0) {
                    if (errno != EWOULDBLOCK) {
                        perror("  recv() failed");
                        closeConn = true;
                    }
                    break;
                }

                if (len == 0) {
                    LOG_DEBUG("Connection closed. thread=%lu", pthread_self());
                    closeConn = true;
                    break;
                }

                LOG_DEBUG("  %d bytes received at thread=%lu", len, pthread_self());

                // todo: remove this because it is echo server
                int returnCode = (int)send(fds[i].fd, buffer, (size_t)len, 0);
                if (returnCode < 0) {
                    perror("  send() failed");
                    closeConn = true;
                    break;
                }

            } while (true);

            if (closeConn) {
                close(fds[i].fd);
                fds[i].fd = -1;
                resizeArray = true;
            }
        }
    }

    if (resizeArray) { //todo improve
        resizeArray = false;
        for (int i = 0; i < nfds; ++i) {
            if (fds[i].fd == -1) {
                for (int j = i; j < nfds; ++j) {
                    fds[j].fd = fds[j + 1].fd;
                }
                --i;
                --nfds;
            }
        }
    }
}
#endif // POLLING_H
