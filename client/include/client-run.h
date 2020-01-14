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
#include "client-outgoing-mail-struct.h"
#include "client-worker-task.h"
#include <sys/wait.h> 
#include <sys/types.h>
#include "client-context.h"
#include "smtp_sockets.h"


#define CLIENT_TASK_DISPATCH_TIMEOUT_SECONDS 360
#define CLIENT_TASK_DISPATCH_TIMEOUT_USECONDS 0

int run(const char *root_dir, const char *outmail_dir, const char *process_dir, const char *sent_dir,
        const char *log_file_name, uint32_t mail_send_timeout, uint32_t mail_retry_wait_time,
        uint32_t number_of_workers);

#endif
