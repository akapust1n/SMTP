#ifndef _CLIENT_RUN_H_
#define _CLIENT_RUN_H_

/** \file client-run.h
 *  \brief Основные функции
 *
 *  В этом файле описана главная функция программы.
 */

#include <stdio.h>
#include <string.h>
#include "client-worker.h"
#include "checkoptn.h"
#include "path-utils.h"
#include <stdlib.h>
#include <stdio.h>
#include "outgoing-mail-struct.h"
#include "client-worker-task.h"
#include <sys/wait.h> 
#include <sys/types.h>

struct smtp_client_context
{
	const char *root_dir;
	char *outmail_dir;
	char *ready_dir;
	char *process_dir;
	char *sent_dir;
	const char *log_file_name;
	uint32_t mail_send_timeout;
	uint32_t mail_retry_wait_time;
	uint32_t number_of_workers;
	bool disable_random_process_dir;
	bool disable_random_file_names;
	uint32_t current_worker;
	uint32_t number_of_mail_sent;
	int logger_socket;
	pid_t logger_pid;
	struct smtp_client_worker_context* worker_ctx;
};

int run(const char *root_dir, const char *outmail_dir, const char *ready_dir,
        const char *process_dir, const char *sent_dir, const char *log_file_name,
        unsigned int mail_send_timeout, unsigned int mail_retry_wait_time,
        unsigned char number_of_workers, bool disable_random_process_dir,
        bool disable_random_file_names);
#endif
