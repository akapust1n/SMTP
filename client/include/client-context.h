#ifndef __CLIENT_CONTEXT_H__
#define __CLIENT_CONTEXT_H__

#include <wait.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/select.h>
#include <linked-list.h>

struct smtp_client_context
{
    const char *root_dir;
    char *outmail_dir;
    char *process_dir;
    char *sent_dir;
    const char *log_file_name;
    uint32_t mail_send_timeout;
    uint32_t mail_retry_wait_time;
    uint32_t number_of_workers;
    uint32_t number_of_mail_sent;
    int logger_socket;
    pid_t logger_pid;
    struct smtp_client_worker_context* worker_ctx;
    fd_set worker_task_fds;
    char* name;
};

struct smtp_client_worker_context
{
    const char *process_dir;
    const char *sent_dir;
    uint32_t mail_send_timeout;
    uint32_t mail_retry_wait_time;
    pid_t pid;
    int master_socket;
    int worker_socket;
    int logger_socket;
    bool is_running;
    char *name;
    struct linked_list *task_queue;
};

#define MAIN_PROCESS_NAME "main"
#define WORKER_PROCESS_NAME "worker[%d]"

#endif

