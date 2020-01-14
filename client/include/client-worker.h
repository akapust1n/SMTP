#ifndef __CLIENT_WORKER_H__
#define __CLIENT_WORKER_H__

#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include "client-fsm.h"
#include "client-context.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "client-logger.h"
#include "client-re.h"


struct response_to_event
{
    enum smtp_server_response response;
    te_client_fsm_event event;
};

int worker_process_run(struct smtp_client_worker_context* ctx);


#endif
