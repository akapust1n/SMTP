/** \file client-run.c
 *  \brief Основные функции.
 *
 *  В этом файле описана главная функция программы.
 */

#include "client-run.h"
#include "client-logger.h"
#include "client-worker.h"
#include "smtp_sockets.h"
#include "hashtable.h"

int spawn_worker_process(int id, struct smtp_client_context* main_ctx, struct smtp_client_worker_context* worker_ctx)
{
        int sockets[2];

        if (create_local_socket_pair(sockets) != 0)
        {
                perror("Failed to spawn worker process - socket pair creation failed");
                return -1;
        }

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

	worker_ctx->pid = pid;

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

        main_ctx->logger_socket = sockets[0];

        pid_t pid = fork();

        if (pid == 0)
        {
                log_process_run(sockets[1], main_ctx->log_file_name);
        }
	else
        {
                main_ctx->logger_pid = pid;
        }

        return 0;
}

int main_loop(struct smtp_client_context* ctx)
{
        bool running = true;
        
        log_print("Starting to look for new mail to send in maildir");
        struct hashtable *directory_dict = outgoing_mail_dictionary_create();
        
        while (running)
        {
                scan_dir_for_new_mail(ctx, directory_dict);
                sleep(30);
        }
        log_print("Finished main loop");       
        return 0;
}

int scan_dir_for_new_mail(struct smtp_client_context *ctx, struct hashtable *directory_dict)
{
	log_print("Start scanning for new mail");

	outgoing_mail_dictionary_add_files_from_directory(directory_dict, ctx->outmail_dir);
	for (int i = 0; i < directory_dict->current_size; i++)
	{
		if (directory_dict->data[i].length == 0)
		{
			continue;
		}
		log_print("Found mail for %s domain", directory_dict->data[i]->list->key);
		
		struct worker_task *task = NULL;
		task_struct_create(task, directory_dict->data[i]->list);
		dispatch_task_to_worker(ctx);
		task_struct_free(task);
	}
        log_print("Scan finished, going to sleep");
}

int stop_all_worker_processes(struct smtp_client_context* ctx)
{
	log_print("Stopping all workers");

        client_process_command =
        {
                .type = SMTP_CLIENT_PROCESS_STOP,
                .socket = -1
        };

        for (int i = 0; i < ctx->number_of_workers; i++)
        {
                smtp_client_worker_context *worker_ctx = ctx->worker_ctx + i;
                send(worker->master_socket, &command, sizeof(command), 0);
                close(worker->master_socket);
        }

        for (int i = 0; i < ctx->number_of_workers; i++)
        {
                smtp_client_worker_context *worker_ctx = ctx->worker_ctx + i;

                int status;
                waitpid(worker_ctx->pid, &status, 0);
        }

        return 0;
}

int stop_logger_process(struct smtp_client_context* ctx)
{
	log_print("Stopping logger thread");

        client_process_command =
        {
                .type = SMTP_CLIENT_PROCESS_STOP,
                .socket = -1
        };

        send(ctx->logger_socket, &command, sizeof(command), 0);

        int status;
        waitpid(ctx->logger_pid, &status, 0);

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
	uint32_t root_dir_len = strlen(root_dir);

	char *outmail_path = (char*)malloc(0x100);
	strncpy(outmail_path, root_dir, 0x100);
	strncat(outmail_path, outmail_dir, 0x100 - len);
        ctx.outmail_dir = outmail_path;

	char *ready_path = (char*)malloc(0x100);
	strncpy(ready_path, root_dir, 0x100);
	strncat(ready_path, ready_dir, 0x100 - len);
        ctx.ready_dir = ready_path;

	char *process_path = (char*)malloc(0x100);
	strncpy(process_path, root_dir, 0x100);
	strncat(process_path, process_dir, 0x100 - len);
        ctx.process_dir = process_path;
	
	char *sent_path = (char*)malloc(0x100);
	strncpy(sent_path, root_dir, 0x100);
	strncat(sent_path, sent_dir, 0x100 - len);
        ctx.sent_dir = sent_path;

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
        free(ctx.outmail_dir);
	free(ctx.ready_dir);
	free(ctx.process_dir);
        free(ctx.sent_dir);

        return result;
};

