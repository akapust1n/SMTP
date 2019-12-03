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

int run(const char *root_dir, const char *logfile,
	unsigned int mail_send_timeout, unsigned int mail_retry_wait_time,
	unsigned char number_of_workers);
#endif
