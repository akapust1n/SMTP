#ifndef __CLIENT_WORKER_H__
#define __CLIENT_WORKER_H__

#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include "client-fsm.h"
#include "client-context.h"

struct smtp_client_worker_context
{
        const char *root_dir;
	const char *outmail_dir;
	const char *process_dir;
	const char *sent_dir;
	const char *log_file;
	uint32_t mail_send_timeout;
	uint32_t mail_retry_wait_time;
	pid_t pid;
	int master_socket;
	int worker_socket;
	int logger_socket;
	bool is_running;
};


int worker_process_run(struct smtp_client_worker_context* ctx);


#endif
