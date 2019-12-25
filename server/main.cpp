#include "logger.h"
#include "polling.h"
#define PORT 10000

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

    int nfds = 1;
    pollfd fds[100];
    memset(&fds, 0, sizeof(fds));

    fds[0].fd = listenSocket;
    fds[0].events = POLLIN;

    int result = 0;
    do {
        result = doPoll(fds, listenSocket, nfds);
    } while (result < 0);

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
