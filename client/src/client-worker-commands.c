#include <sys/socket.h>
#include <zconf.h>
#include "client-worker-commands.h"

int send_terminate_to_worker(struct smtp_client_worker_context *worker_ctx)
{
    struct client_process_command command =
    {
        .type = SMTP_CLIENT_PROCESS_STOP
    };

    send(worker_ctx->master_socket, &command, sizeof(command), 0);
    close(worker_ctx->master_socket);
}

int send_task_to_worker(struct smtp_client_worker_context *worker_ctx, const void *task, size_t task_size)
{
    struct client_process_command command =
    {
       .type = SMTP_CLIENT_TASK
    };

    send(worker_ctx->master_socket, &command, sizeof(command), 0);
    send(worker_ctx->master_socket, &task_size, sizeof(task_size), 0);
    send(worker_ctx->master_socket, task, task_size, 0);
}
