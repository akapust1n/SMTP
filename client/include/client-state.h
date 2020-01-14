#ifndef _CLIENT_STATE_H_
#define _CLIENT_STATE_H_

#include "client-fsm.h"

struct client_state_t {
	te_client_fsm_state state;
	unsigned int number_of_attempts;
};

void init_state(struct client_state_t *state);
//void free_state(struct client)state_t *state);

#endif
