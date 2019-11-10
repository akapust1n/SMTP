#include "logger.h"

#define PORT 10000
#define MAX_CLIENTS 50

struct argsForThread {
    int listenSocket;
};

int initSocket(int* listenSocket)
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

void* myPoll(void* _args)
{
    struct argsForThread args = *(struct argsForThread*)_args;
    int listenSocket = args.listenSocket;
    LOG_DEBUG("Starting working thread=%lu", pthread_self());

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
        LOG_DEBUG("Waiting on poll() at thread=%lu \n", pthread_self());
        int returnCode = poll(fds, (nfds_t)nfds, timeout);

        if (returnCode < 0) {
            LOG_DEBUG("  poll() failed");
            break;
        }

        if (returnCode == 0) {
            LOG_DEBUG("  poll() timed out.  End program.\n");
            break;
        }

        currentSize = nfds;
        for (int i = 0; i < currentSize; ++i) {
            if (fds[i].revents == 0)
                continue;

            if (!(fds[i].revents & POLLIN)) {
                LOG_DEBUG("Error! revents=%d thread=%lu", fds[i].revents, pthread_self());
                invalidStateOfServer = true;
                break;
            }
            if (fds[i].fd == listenSocket) {
                LOG_DEBUG(" Listening socket is readable. Socket=%d thread=%lu", listenSocket, pthread_self());
                if (nfds >= MAX_CLIENTS) {
                    LOG_DEBUG("reach limit of client thread=%lu", pthread_self());
                    continue;
                }
                do {
                    newSocket = accept(listenSocket, NULL, NULL);
                    if (newSocket < 0) {
                        /* if (errno != EWOULDBLOCK) {
                            perror("  accept() failed");
                            invalidStateOfServer = true;
                        }*/
                        //todo catch more errors
                        continue;
                    }

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
    struct argsForThread args = { .listenSocket = -1 };

    if (initSocket(&(args.listenSocket)) < 0) {
        perror("Cant open socket");
        return -1;
    }

    if (mq_unlink(MQ_QUEUE) < 0) {
        printf("Warning %d (%s) on server mq_unlink.\n",
            errno, strerror(errno)); //will write on first start
    }

    struct mq_attr attr = { .mq_maxmsg = 10, .mq_msgsize = bufSize };
    mqd_t msqid = mq_open(MQ_QUEUE, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR, &attr);

    if (msqid < 0) {
        perror("mq_open error");
        printf("Warning %d (%s) on server mq_open.\n",
            errno, strerror(errno)); //will write on first start

        return -1;
    }

    pthread_t tid1, tid2, tidLogger;

    pthread_create(&tid1, 0, myPoll, &args);
    pthread_create(&tid2, 0, myPoll, &args);
    pthread_create(&tidLogger, 0, loggerMain, &msqid);

    printf("staring logger... \n");
    pthread_detach(tidLogger);
    pthread_detach(tid1);
    pthread_join(tid2, NULL);

    mq_close(msqid);
    return 0;
}
