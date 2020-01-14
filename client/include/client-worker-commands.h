#ifndef __CLIENT_COMMANDS_H__
#define __CLIENT_COMMANDS_H__

#include <glob.h>
#include <wait.h>
#include "client-context.h"

enum task_commands
{
    SMTP_CLIENT_TASK,
    SMTP_CLIENT_LOG_PRINT,
    SMTP_CLIENT_PROCESS_STOP
};

struct client_process_command
{
    enum task_commands type;
};

int send_terminate_to_worker(struct smtp_client_worker_context *worker_ctx);
int send_task_to_worker(struct smtp_client_worker_context *worker_ctx, void *task, size_t task_size);

#endif
