/** \file client-run.c
 *  \brief Основные функции.
 *
 *  В этом файле описана главная функция программы.
 */

#include "client-run.h"

int spawn_worker_process(int id, struct smtp_client_context* main_ctx, struct smtp_client_worker_context* worker_ctx)
{
        int sockets[2];

        if (create_local_socket_pair(sockets) != 0)
        {
                perror("Failed to spawn worker process - socket pair creation failed");
                return -1;
        }

        worker_ctx->id = id;
        worker_ctx->root_dir = &main_ctx->root_dir;
        worker_ctx->process_dir = &main_ctx->process_dir;
        worker_ctx->sent_dir = &main_ctx->sent_dir;
        worker_ctx->master_socket = sockets[0];
        worker_ctx->worker_socket = sockets[1];
        worket_ctx->logger_socket = main_ctx->logger_socket;
        worker_ctx->disable_random_process_dir = ctx->disable_random_process_dir;
        worker_ctx->disable_random_file_names = ctx->disable_random_file_names;
        worker_ctx->mail_send_timeout = ctx->mail_send_timeout;
        worker_ctx->mail_retry_wait_time = ctx->mail_retry_wait_time;
        worker_ctx->is_running = true;
        
        pid_t pid = fork();

        if (pid == 0)
        {
                worker_process_run(worker_ctx);
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

        main_ctx->sockets[0];

        pid_t pid = fork();

        if (pid == 0)
        {
                log_process_run(sockets[1], main_ctx->log_file_name);
        }

        return 0;
}

int run(const char *root_dir, const char *outmail_dir, const char *ready_dir,
        const char *process_dir, const char *sent_dir, const char *log_file_name,
        unsigned int mail_send_timeout, unsigned int mail_retry_wait_time,
        unsigned char number_of_workers, bool disable_random_process_dir,
        bool disable_random_file_names)
{
	int result = 0;
        struct smtp_client_context ctx;

        // initialize client context
        ctx.root_dir = root_dir;
        ctx.outmail_dir = outmail_dir;
        ctx.ready_dir = ready_dir;
        ctx.process_dir = process_dir;
        ctx.sent_dir = sent_dir;
        ctx.log_file_name = log_file_name;
        ctx.mail_send_timeout = mail_send_timeout;
        ctx.mail_retry_wait_time = mail_retry_wait_time;
        ctx.number_of_workers = number_of_workers;
        ctx.disable_random_process_dir = disable_random_process_dir;
        ctx.disable_random_file_names = disable_random_file_names;
        ctx.worker_contexts = (struct smtp_client_worker_context*)malloc(sizeof(struct smtp_client_worker_context) * number_of_workers);

        spawn_logger_process(&ctx);

        log_print("Starting SMTP client");
        log_print("Number of worker processes: %d");
        
        //spawn worker processes
        for (unsigned int i = 0; i < number_of_workers; i++)
        {
                if (spawn_worker_process(i, &ctx, ctx.worker_contexts[i]) != 0)
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

        free(ctx.worker_contexts);
        
        return result;
};

