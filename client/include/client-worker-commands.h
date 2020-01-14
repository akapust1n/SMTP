#ifndef __CLIENT_COMMANDS_H__
#define __CLIENT_COMMANDS_H__

#include <glob.h>
#include <wait.h>

enum task_commands
{
    SMTP_CLIENT_TASK,
    SMTP_CLIENT_PROCESS_STOP
};

struct client_process_command
{
    enum task_commands type;
};

int send_terminate_to_worker(pid_t worker);
int send_task_to_worker(pid_t worker, void *data, size_t data_size);

#endif
