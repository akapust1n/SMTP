#include <stdio.h>
#include <zconf.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <client-worker-commands.h>
#include "client-logger.h"
#include "client-run.h"

int logger_socket;

static char *create_log_message(const char* name, const char* format, va_list args)
{
    char current_time_string[32];
    time_t current_time = time(NULL);

    strftime(current_time_string, 32, "%Y-%m-%d %H:%M:%S", localtime(&current_time));

    int len = vsnprintf(NULL, 0, format, args);
    char * tmp_buffer = (char *)malloc(len+1);
    vsnprintf(tmp_buffer, len+1, format, args);

    len = snprintf(NULL, 0, "%s [%s]: %s", current_time_string, name, tmp_buffer);
    char *result = malloc(len+1);
    snprintf(result, len+1, "%s [%s]: %s", current_time_string, name, tmp_buffer);

    free(tmp_buffer);

    return result;
}

int log_print(const char* name, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char *message = create_log_message(name, format, args);
    va_end(args);

    size_t len = strlen(message);

    struct client_process_command command =
            {
                    .type = SMTP_CLIENT_LOG_PRINT
            };

    write(logger_socket, &command, sizeof(command));
    write(logger_socket, &len, sizeof(len));
    write(logger_socket, message, len);
    free(message);

    return 0;
}

int log_process_send_terminate()
{
    struct client_process_command command =
            {
                    .type = SMTP_CLIENT_PROCESS_STOP
            };

    write(logger_socket, &command, sizeof(command));
    close(logger_socket);
    return 0;
}

int log_process_run(int socket, const char *logfile)
{
    return 0;
}

