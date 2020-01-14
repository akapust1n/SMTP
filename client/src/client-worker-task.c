#include "client-worker-task.h"

struct linked_list *create_new_task_queue()
{
	struct linked_list *new_task_queue = linked_list_create();

	return new_task_queue;
}

struct worker_task *get_next_task(struct linked_list *tasks)
{
	struct worker_task *next_task = (struct worker_task *)linked_list_pop(tasks);

	return next_task;
}

struct worker_task *get_task_from_node(struct linked_list_node *node)
{
	if (node == NULL)
	{
		return NULL;
	}
	return (struct worker_task *)(node->data);
}

void add_task(struct linked_list *tasks, const char* domain)
{
	struct worker_task new_task;
	new_task.domain = (char*)malloc(strlen(domain));

	memcpy(new_task.domain, domain, strlen(domain));
	new_task.state = TASK_WAINTING_IN_QUEUE;
	linked_list_push(tasks, &new_task, sizeof(new_task));
}

void remove_all_finished_tasks_from_queue(struct linked_list *tasks)
{
	struct linked_list_node *current_node = tasks->head;
	uint32_t index = 0;
	while(current_node != NULL)
	{
		if (((struct worker_task *)current_node->data)->state == TASK_FINISHED)
		{
			current_node = current_node->next;
			linked_list_remove(tasks, index);
		}
		else
		{
			index++;
			current_node = current_node->next;
		}
	}
}
