#ifndef __CLIENT_LOGGER_H__
#define __CLIENT_LOGGER_H__

extern int logger_socket;

#define LOGGER_BUFFER_SIZE 512

int log_print(const char *name, const char *format, ...);
int log_process_run(int socket, const char *logfile);
int log_process_send_terminate();

#endif
