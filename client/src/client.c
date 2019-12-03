/** \file client.c
 *  \brief Основные функции
 *
 *  Этот файл содержит точку входа.
 */

 #include <stdio.h>
 #include <string.h>

 #include "checkoptn.h"
 #include "client-run.h"


 /** \fn int main(int argc, char *argv[])
  *  \brief Точка входа в программу.
  *  \param argc -- число аргументов при вызове программы;
  *  \param argv -- аргументы вызова программы;
  *  \return код завершения программы;
  */

int main(int argc, char *argv[])
{
	int optct = optionProcess(&smtp_clientOptions, argc, argv);

	argc -= optct;
	argv += optct;

	return run(OPT_ARG(ROOT_DIR), OPT_ARG(LOG_FILE_NAME), OPT_ARG(MAIL_SEND_TIMEOUT),
                OPT_ARG(MAIL_RETRY_WAIT_TIME), OPT_ARG(NUMBER_OF_WORKERS));
}

