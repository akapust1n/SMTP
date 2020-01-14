#include <client-worker-task.h>
#include <hashtable.h>
#include <client-outgoing-mail-struct.h>
#include "client-worker.h"

static struct response_to_event response_to_event_table[] =
{
        (struct response_to_event){SERVER_UNKNOWN_RESPONSE, CLIENT_FSM_EV_EVENT_ERROR},
        (struct response_to_event){SERVER_CONNECT, CLIENT_FSM_EV_EVENT_CONNECTED},
        (struct response_to_event){SERVER_EHLO_OK_MULTILINE, CLIENT_FSM_EV_EVENT_GOT_EHLO_RESPONSE_MULTILINE},
        (struct response_to_event){SERVER_OK, CLIENT_FSM_EV_EVENT_GOT_OK},
        (struct response_to_event){SERVER_EHLO_NOT_SUPPORTED, CLIENT_FSM_EV_EVENT_GOT_EHLO_NOT_SUPPORTED},
        (struct response_to_event){SERVER_DATA_OK, CLIENT_FSM_EV_EVENT_GOT_DATA_RESPONSE},
        (struct response_to_event){SERVER_QUIT_OK, CLIENT_FSM_EV_EVENT_GOT_QUIT_RESPONSE},
        (struct response_to_event){SERVER_ERROR, CLIENT_FSM_EV_EVENT_ERROR},
};

int recieve_new_task(struct smtp_client_worker_context* ctx)
{
    return 0;
}

int create_new_connection(struct smtp_client_worker_context* ctx, fd_set *read_fds, fd_set *write_fds,
        struct hashtable *sockets)
{
    return 0;
}

int read_from_server(struct smtp_client_worker_context* ctx, int server_socket, struct worker_task *current_task)
{
    return 0;
}

int write_to_server(struct smtp_client_worker_context* ctx, int server_socket, struct worker_task *current_task)
{
    return 0;
}

int worker_process_run(struct smtp_client_worker_context* ctx)
{
    size_t len = strlen(WORKER_PROCESS_NAME) + 20;
    ctx->name = (char *)malloc(len);
    snprintf(ctx->name, len, WORKER_PROCESS_NAME, getpid());
    fd_set read_fds, write_fds;
    fd_set current_read_fds, current_write_fds;
    log_print(ctx->name, "starting worker loop");
    ctx->task_queue = create_new_task_queue();
    struct hashtable * server_sockets = outgoing_mail_dictionary_create();

    FD_ZERO(&read_fds);
    FD_ZERO(&current_read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&current_write_fds);

    FD_SET(ctx->worker_socket, &read_fds);
    uint32_t max_fds = 1;

    while(ctx->is_running)
    {
        memcpy(&current_read_fds, &read_fds, sizeof(read_fds));
        memcpy(&current_write_fds, &write_fds, sizeof(read_fds));

        int result = select(max_fds + 1, &current_read_fds, &current_write_fds, NULL, NULL);

        switch (result)
        {
            case -1:
                log_print(ctx->name, "select() failed");
                ctx->is_running = false;
                break;
            case 0:
                log_print(ctx->name, "select() returned 0");
                ctx->is_running = false;
                break;
            default:

                if (FD_ISSET(ctx->worker_socket, &current_read_fds))
                {
                    recieve_new_task(ctx);
                    create_new_connection(ctx, &read_fds, &write_fds, server_sockets);
                }

                struct linked_list_node *current_task_node = ctx->task_queue->head;

                while(current_task_node != NULL)
                {
                    struct worker_task * current_task = get_task_from_node(current_task_node);
                    int server_socket = hashtable_get(server_sockets,
                                                      current_task->domain.strlen(current_task->domain));
                    if (current_task->expect_reads_from_socket)
                    {
                        if (FD_ISSET(server_socket, &current_read_fds))
                        {
                            read_from_server(ctx, server_socket, current_task);
                        }
                    }
                    else
                    {
                        if (FD_ISSET(server_socket, &current_write_fds))
                        {
                            write_to_server(ctx, server_socket, current_task);
                        }
                    }
                    current_task_node = current_task_node->next;
                }
                remove_all_finished_tasks_from_queue(ctx->task_queue);
        }
    }

    return 0;
}

