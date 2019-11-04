#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define PORT 10000
#define MAX_CLIENTS 50

int initSocker(int* listenSocket)
{
    int temp = 1;

    *listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (*listenSocket < 0) {
        perror("socket() failed");
        return -1;
    }

    if (setsockopt(*listenSocket, SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR,
            (char*)&temp, sizeof(temp))
        < 0) {
        perror("setsockopt() failed");
        close(*listenSocket);
        return -1;
    }

    if (ioctl(*listenSocket, FIONBIO, (char*)&temp) < 0) {
        perror("ioctl() failed");
        close(*listenSocket);
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
    if (bind(*listenSocket,
            (struct sockaddr*)&addr, sizeof(addr))
        < 0) {
        perror("bind() failed");
        close(*listenSocket);
        return -1;
    }

    if (listen(*listenSocket, 3) < 0) {
        perror("listen() failed");
        close(*listenSocket);
        return -1;
    }
    return 0;
}
void* myPoll(void* lSocket)
{
    int listenSocket = *(int*)lSocket;
    bool closeConn = false;
    char buffer[100];
    int timeout = 3 * 60 * 1000; // 3min
    bool invalidStateOfServer = false, resizeArray = false;
    int newSocket = -1;
    int nfds = 1, currentSize = 0;
    struct pollfd fds[100];
    memset(&fds, 0, sizeof(fds));

    fds[0].fd = listenSocket;
    fds[0].events = POLLIN;

    do {
        printf("Waiting on poll() at thread %lu \n", pthread_self());
        int returnCode = poll(fds, (nfds_t)nfds, timeout);

        if (returnCode < 0) {
            perror("  poll() failed");
            break;
        }

        if (returnCode == 0) {
            printf("  poll() timed out.  End program.\n");
            break;
        }

        currentSize = nfds;
        for (int i = 0; i < currentSize; ++i) {
            if (fds[i].revents == 0)
                continue;

            if (!(fds[i].revents & POLLIN)) {
                printf("  Error! revents = %d\n", fds[i].revents);
                invalidStateOfServer = true;
                break;
            }
            if (fds[i].fd == listenSocket) {
                printf("  Listening socket is readable\n");
                if (nfds >= MAX_CLIENTS) {
                    printf("reach limit of client\n");
                    continue;
                }
                do {
                    newSocket = accept(listenSocket, NULL, NULL);
                    if (newSocket < 0) {
                       /* if (errno != EWOULDBLOCK) {
                            perror("  accept() failed");
                            invalidStateOfServer = true;
                        }*/ //todo catch more errors
                        continue;
                    }

                    printf("  New incoming connection - %d\n", newSocket);
                    fds[nfds].fd = newSocket;
                    fds[nfds].events = POLLIN;
                    ++nfds;

                } while (newSocket != -1);
            }

            else {
                printf("  Descriptor %d is readable\n", fds[i].fd);
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
                        printf("  Connection closed\n");
                        closeConn = true;
                        break;
                    }

                    printf("  %d bytes received at thread %lu \n", len, pthread_self());

                    // todo: remove this
                    returnCode = (int)send(fds[i].fd, buffer, (size_t)len, 0);
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

    } while (!invalidStateOfServer);

    for (int i = 0; i < nfds; ++i) {
        if (fds[i].fd >= 0)
            close(fds[i].fd);
    }
    pthread_exit(0);
}
int main()
{
    int listenSocket = -1;

    if (initSocker(&listenSocket) < 0) {
        perror("Cant open socket");
        return -1;
    }
    pthread_t tid1, tid2;

    /* создание потока */
    pthread_create(&tid1, 0, myPoll, &listenSocket);
    pthread_create(&tid2, 0, myPoll, &listenSocket);
    printf("Starting thread #1\n");
    pthread_detach(tid1);
    printf("Starting thread #2\n");
    pthread_join(tid2, NULL);

    return  0;
}
