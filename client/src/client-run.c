/** \file client-run.c
 *  \brief Основные функции.
 *
 *  В этом файле описана главная функция программы.
 */

#include <client-worker-commands.h>
#include <time.h>
#include "client-run.h"
#include "client-logger.h"
#include "client-worker.h"
#include "smtp_sockets.h"
#include "hashtable.h"

int scan_dir_for_new_mail(struct smtp_client_context *ctx, struct hashtable *directory_dict);
int spawn_worker_process(struct smtp_client_context* main_ctx, struct smtp_client_worker_context* worker_ctx);
int spawn_logger_process(struct smtp_client_context* main_ctx);
int dispatch_task_to_worker(struct smtp_client_context* ctx, struct hashtable_node_list *list);
int initialize_client_fd_set(struct smtp_client_context* ctx);
int initialize_directories(struct smtp_client_context* ctx);

bool main_process_running = true;

int spawn_worker_process(struct smtp_client_context* main_ctx, struct smtp_client_worker_context* worker_ctx)
{
    int sockets[2];

    if (create_local_socket_pair(sockets) != 0)
    {
        log_print(main_ctx->name, "Failed to spawn worker process - socket pair creation failed");
        return -1;
    }

    worker_ctx->root_dir = main_ctx->root_dir;
    worker_ctx->process_dir = main_ctx->process_dir;
    worker_ctx->sent_dir = main_ctx->sent_dir;
    worker_ctx->master_socket = sockets[0];
    worker_ctx->worker_socket = sockets[1];
    worker_ctx->logger_socket = main_ctx->logger_socket;
    worker_ctx->mail_send_timeout = main_ctx->mail_send_timeout;
    worker_ctx->mail_retry_wait_time = main_ctx->mail_retry_wait_time;
    worker_ctx->is_running = true;
        
    pid_t pid = fork();

    worker_ctx->pid = pid;

    if (pid == 0)
    {
            worker_process_run(worker_ctx);
	    logger_socket = sockets[0];
    }

    return 0;
}

int spawn_logger_process(struct smtp_client_context* main_ctx)
{
    int sockets[2];

    if (create_local_socket_pair(sockets) != 0)
    {
            perror("Failed to spawn logger process - socket pair creation failed");
            return -1;
    }

    main_ctx->logger_socket = sockets[0];

    pid_t pid = fork();

    if (pid == 0)
    {
        log_process_run(sockets[1], main_ctx->log_file_name);
    }
    else
    {
        main_ctx->logger_pid = pid;
        logger_socket = sockets[0];
    }

    return 0;
}

int initialize_client_fd_set(struct smtp_client_context* ctx)
{
    fd_set write_sd;

    FD_ZERO(&write_sd);
    for (uint32_t i = 0; i < ctx->number_of_workers; i++)
    {
        int socket = ctx->worker_ctx[i].master_socket;
        socket_set_nonblocking(socket, true);
        FD_SET(socket, &write_sd);
    }
    memccpy(&ctx->worker_task_fds, &write_sd, sizeof(write_sd));
}

int initialize_directories(struct smtp_client_context *ctx)
{
    log_print(ctx->name, "Checking if directory '%s' exists", ctx->root_dir);
    if (!check_if_directory_exists(ctx->root_dir))
    {
        if(!create_path(ctx->root_dir, 0666))
        {
	    log_print(ctx->name, "Unable to create directory %s", ctx->root_dir);
        }
    }
    if (!check_if_directory_exists(ctx->process_dir))
    {
	log_print(ctx->name, "Creating '%s' directory", ctx->root_dir);
        if(!create_path(ctx->process_dir, 0666))
        {
	    log_print(ctx->name, "Unable to create directory '%s'", ctx->process_dir);
        }
    }
    log_print(ctx->name, "Checking if directory '%s' exists", ctx->process_dir);
    if (!check_if_directory_exists(ctx->sent_dir))
    {
	log_print(ctx->name, "Creating '%s' directory", ctx->process_dir);
        if(!create_path(ctx->sent_dir, 0666))
        {
            log_print(ctx->name, "Unable to create directory '%s'", ctx->sent_dir);
        }
    }

    for (uint32_t i = 0; i < ctx->number_of_workers; i++)
    {
        char buffer[20];
        struct smtp_client_worker_context *worker_ctx = ctx->worker_ctx + i;

        log_print(ctx->name, "Creating directory for pid: %d", worker_ctx->pid);

        snprintf(buffer, 20, "%d", worker_ctx->pid);
        create_subdirectory(ctx->process_dir, buffer, 0666);
    }
}

int main_loop(struct smtp_client_context* ctx)
{
    log_print(ctx->name, "Starting to look for new mail to send in maildir");
    struct hashtable *directory_dict = outgoing_mail_dictionary_create();
    initialize_client_fd_set(ctx);
    initialize_directories(ctx);

    while (main_process_running)
    {
        scan_dir_for_new_mail(ctx, directory_dict);
        sleep(30);
    }
    log_print(ctx->name, "Finished main loop");
    return 0;
}

int scan_dir_for_new_mail(struct smtp_client_context *ctx, struct hashtable *directory_dict)
{
	log_print(ctx->name, "Start scanning for new mail");

	outgoing_mail_dictionary_add_files_from_directory(directory_dict, ctx->outmail_dir);
	for (int i = 0; i < directory_dict->current_size; i++)
	{
		if (directory_dict->data[i].list_length == 0)
		{
			continue;
		}
		log_print(ctx->name, "Found mail for %s domain", directory_dict->data[i].list->key);
		
		dispatch_task_to_worker(ctx, directory_dict->data);
	}
        log_print(ctx->name, "Scan finished, going to sleep");
}

char *generate_filename(struct smtp_client_context* ctx, const char *domain)
{
    char *result = NULL;
    char current_time_string[32];
    time_t current_time = time(NULL);

    strftime(current_time_string, 32, "%Y%m%d%H%M%S", localtime(&current_time));

    int len = snprintf(result, 0, "%s%s%d", current_time_string, domain, ctx->number_of_mail_sent);
    result = (char *)malloc(len + 1);
    snprintf(result, len + 1, "%s%s%d", current_time_string, domain, ctx->number_of_mail_sent);

    ctx->number_of_mail_sent++;

    return result;
}

int dispatch_task_to_worker(struct smtp_client_context* ctx, struct hashtable_node_list *list)
{
    int write_socket;
    log_print(ctx->name, "Waiting on select()...");

    write_socket = select(ctx->number_of_workers + 1, NULL, ctx->worker_task_fds, NULL, NULL);
    if (write_socket < 0)
    {
        log_print(ctx->name, "select() failed");
        return -1;
    }

    char *path = NULL;
    struct smtp_client_worker_context *worker_ctx = ctx->worker_ctx + write_socket;
    int len = snprintf(path, 0, "%s/%d", ctx->process_dir, worker_ctx->pid);

    path = (char *)malloc(len + 1);
    snprintf(path, len + 1, "%s/%d", ctx->process_dir, worker_ctx->pid);
    const char *domain = list->list->key;
    size_t domain_len = list->list->key_size;

    struct hashtable_node *current_node = list->list;

    for (uint32_t i = 0; i < list->list_length; i++)
    {
        if (strncmp(domain, current_node->key, domain_len) == 0)
        {
            char *path_to_file = NULL;
            char * filename = generate_filename(ctx, domain);
            len = snprintf(path_to_file, 0, "%s/%s/%s", path, domain, filename);
            path_to_file = (char *)malloc(len + 1);
            snprintf(path_to_file, len + 1, "%s/%s/%s", path, domain, filename);
            log_print(ctx->name, "Moving '%s' to '%s'", current_node->value, path_to_file);
            if (rename(current_node->value, path_to_file) != 0)
            {
                log_print(ctx->name, "Moving failed");
            }
            free(path_to_file);
            free(filename);
        }
    }

    send_task_to_worker(worker_ctx, domain, domain_len);
    free(path);

    return 0;
}

int stop_all_worker_processes(struct smtp_client_context* ctx)
{
    log_print("Stopping all workers");

    struct client_process_command command =
    {
        .type = SMTP_CLIENT_PROCESS_STOP
    };

    for (int i = 0; i < ctx->number_of_workers; i++)
    {
        struct smtp_client_worker_context *worker_ctx = ctx->worker_ctx + i;
        send_terminate_to_worker(worker_ctx);
        int status;
        waitpid(worker_ctx->pid, &status, 0);
    }

    return 0;
}

int stop_logger_process(struct smtp_client_context* ctx)
{
    log_print("Stopping logger thread");
    log_process_send_terminate();
    int status;
    waitpid(ctx->logger_pid, &status, 0);
    return 0;
}

int run(const char *root_dir, const char *outmail_dir, const char *process_dir, const char *sent_dir,
        const char *log_file_name, unsigned int mail_send_timeout, unsigned int mail_retry_wait_time,
        unsigned char number_of_workers)
{
    int result = 0;
    struct smtp_client_context ctx;

    // initialize client context
    ctx.root_dir = root_dir;
    size_t root_dir_len = strlen(root_dir);

    char *outmail_path = (char*)malloc(0x100);
    strncpy(outmail_path, root_dir, 0x100);
    strncat(outmail_path, outmail_dir, 0x100 - root_dir_len);
    ctx.outmail_dir = outmail_path;

    char *process_path = (char*)malloc(0x100);
    strncpy(process_path, root_dir, 0x100);
    strncat(process_path, process_dir, 0x100 - root_dir_len);
    ctx.process_dir = process_path;
	
    char *sent_path = (char*)malloc(0x100);
    strncpy(sent_path, root_dir, 0x100);
    strncat(sent_path, sent_dir, 0x100 - root_dir_len);

    ctx.sent_dir = sent_path;

    ctx.log_file_name = log_file_name;
    ctx.mail_send_timeout = mail_send_timeout;
    ctx.mail_retry_wait_time = mail_retry_wait_time;
    ctx.number_of_workers = number_of_workers;
    ctx.worker_ctx = (struct smtp_client_worker_context*)malloc(sizeof(struct smtp_client_worker_context) * number_of_workers);
    ctx.current_worker = 0;
    ctx.number_of_mail_sent = 0;

   spawn_logger_process(&ctx);

    log_print("Starting SMTP client");
    log_print("Number of worker processes: %d");

    //spawn worker processes
    for (unsigned int i = 0; i < number_of_workers; i++)
    {
        if (spawn_worker_process(&ctx, ctx.worker_ctx + i) != 0)
        {
            log_print("Spawning worker process failed (id=%d)", i);
            return -1;
        }
    }

    log_print("All worker processes started");

    log_print("Start main loop");
    main_loop(&ctx);
    log_print("Quitted main loop");

    log_print("Stopping all workers");
    stop_all_worker_processes(&ctx);
    log_print("Stopping logger");
    stop_logger_process(&ctx);

    free(ctx.worker_ctx);
    free(ctx.outmail_dir);
    free(ctx.process_dir);
    free(ctx.sent_dir);

    return result;
};

