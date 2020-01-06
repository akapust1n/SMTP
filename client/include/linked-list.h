#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

struct linked_list
{
	struct linked_list_node *head;
	uint32_t length;
}

struct linked_list_node
{
	struct linked_list_node *next;
	void *data;
	uint32_t data_size;
}

struct linked_list *linked_list_create();
void linked_list_free(struct linked_list *list);

void *linked_list_get(const struct linked_list *list, uint32_t index);
int linked_list_insert(struct linked_list *list, uint32_t index, void *data, uint32_t data_size);
void linked_list_remove(struct linked_list *list, uint32_t index);

int linked_list_push(struct linked_list *list, void *data, uint32_t data_size);
void *linked_list_pop(struct linked_list *list);

#endif
