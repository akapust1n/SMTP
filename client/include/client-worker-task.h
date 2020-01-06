#ifndef __CLIENT_WORKER_TASK_H__
#define __CLIENT_WORKER_TASK_H__

#include "linked-list.h"


enum task_state
{
	waiting_in_queue,
	running,
	finished
}

struct worker_task
{
	char *domain;
	enum task_state state;
}

struct linked_list *create_new_task_queue();
struct worker_task *get_next_task(struct linked_list *tasks);
void add_task(struct linked_list *tasks, const char* domain);
void remove_all_finished_tasks(struct linked_list *tasks);

#endif
