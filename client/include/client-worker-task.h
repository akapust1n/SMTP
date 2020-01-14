#ifndef __CLIENT_WORKER_TASK_H__
#define __CLIENT_WORKER_TASK_H__

#include "linked-list.h"
#include "client-fsm.h"
#include <stdlib.h>
#include <string.h>

enum task_state
{
	TASK_WAINTING_IN_QUEUE,
	TASK_RUNNING,
	TASK_FINISHED
};

struct worker_task
{
	char *domain;
	char *directory;
	uint32_t number_of_reties;
	int socket;
	bool expect_reads_from_socket;
	te_client_fsm_state current_state;
	enum task_state state;
};

struct linked_list *create_new_task_queue();
struct worker_task *get_next_task(struct linked_list *tasks);
void add_task_to_queue(struct linked_list *tasks, const char* domain);
void remove_all_finished_tasks_from_queue(struct linked_list *tasks);
struct worker_task *get_task_from_node(struct linked_list_node *node);

#endif
