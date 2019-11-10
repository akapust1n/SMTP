#ifndef LOGGER_H
#define LOGGER_H
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/msg.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define LOG_DEBUG(...) LOG(__FILE__, __LINE__, __VA_ARGS__)
#define bufSize 500
#define MQ_QUEUE "/mq_queue11"

void LOG(const char* file, int line, const char* fmt, ...);
void* loggerMain(void* _msqid);
#endif // LOGGER_H
