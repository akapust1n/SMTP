#include "logger.h"

void LOG(const char* file, int line, const char* fmt, ...)
{
    char text[bufSize];
    memset(text, 0x00, sizeof(text));
    time_t t = time(NULL);
    struct tm* lt = localtime(&t);

    char timeBuf[16];
    timeBuf[strftime(timeBuf, sizeof(timeBuf), "%H:%M:%S", lt)] = '\0';
    sprintf(text, "%s %s:%d: ", timeBuf, file, line);
    va_list args;
    va_start(args, fmt);
    vsprintf(text + strlen(text), fmt, args);
    va_end(args);

    int msqid = mq_open(MQ_QUEUE, O_RDWR);
    if (msqid < 0) {
        perror("cant open mq_queue");
        return;
    }

    if (mq_send(msqid, text, sizeof(text), 1) < 0) {
        perror("cant send message to queue");
        mq_close(msqid);
        return;
    }
    mq_close(msqid);
}

void* loggerMain(void* _msqid)
{
    mqd_t msqid = *(mqd_t*)_msqid;

    char buffer[bufSize + 10];
    //struct timespec poll_sleep = ((struct timespec){2, 500000000});

    while (1) {
        memset(buffer, 0x00, sizeof(buffer));
        ssize_t bytes_read = mq_receive(msqid, buffer, bufSize + 10, NULL);
        if (bytes_read >= 0) {
            printf("%s\n", buffer);
            fflush(stdout);
        } else {
            //  nanosleep(&poll_sleep, NULL);
        }
    };
}
