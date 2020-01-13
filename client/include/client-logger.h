#ifndef __CLIENT_LOGGER_H__
#define __CLIENT_LOGGER_H__

int log_print(char *format, ...);
int log_process_run(int socket, const char *logfile);

#endif
