#ifndef _CLIENT_RUN_H_
#define _CLIENT_RUN_H_

/** \file client-run.h
 *  \brief Основные функции
 *
 *  В этом файле описана главная функция программы.
 */

#include <stdio.h>
#include <string.h>

#include "checkoptn.h"

struct smtp_client_context
{
	const char *root_dir;
	const char *outmail_dir;
	const char *ready_dir;
	const char *process_dir;
	const char *sent_dir;
	const char *log_file;
	unsigned int mail_send_timeout;
	unsigned int mail_retry_wait_time;
	unsigned char number_of_workers;
	bool disable_random_process_dir;
	bool disable_random_file_names;
};

int run(const char *root_dir, const char *outmail_dir, const char *ready_dir,
        const char *process_dir, const char *sent_dir, const char *log_file_name,
        unsigned int mail_send_timeout, unsigned int mail_retry_wait_time,
        unsigned char number_of_workers, bool disable_random_process_dir,
        bool disable_random_file_names);
#endif
