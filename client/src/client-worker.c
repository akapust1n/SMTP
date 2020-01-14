#include "client-worker.h"

static struct response_to_event response_to_event_table[] =
{
        (struct response_to_event){SERVER_UNKNOWN_RESPONSE, CLIENT_FSM_EV_EVENT_ERROR},
        (struct response_to_event){SERVER_CONNECT, CLIENT_FSM_EV_EVENT_CONNECTED},
        (struct response_to_event){SERVER_EHLO_OK_MULTILINE, CLIENT_FSM_EV_EVENT_GOT_EHLO_RESPONSE_MULTILINE},
        (struct response_to_event){SERVER_OK, CLIENT_FSM_EV_EVENT_GOT_OK},
        (struct response_to_event){SERVER_EHLO_NOT_SUPPORTED, CLIENT_FSM_EV_EVENT_GOT_EHLO_NOT_SUPPORTED},
        (struct response_to_event){SERVER_DATA_OK, CLIENT_FSM_EV_EVENT_GOT_DATA_RESPONSE},
        (struct response_to_event){SERVER_QUIT_OK, CLIENT_FSM_EV_EVENT_GOT_QUIT_RESPONSE},
        (struct response_to_event){SERVER_ERROR, CLIENT_FSM_EV_EVENT_ERROR},
};

int worker_process_run(struct smtp_client_worker_context* ctx)
{
    size_t len = strlen(WORKER_PROCESS_NAME) + 20;
    ctx->name = (char *)malloc(len);
    snprintf(ctx->name, len, WORKER_PROCESS_NAME, getpid());

    log_print(ctx->name, "starting worker loop");
    ctx->current_state = CLIENT_FSM_ST_INIT;

    while(ctx->is_running)
    {

    }

    return 0;
}

