#include "linked-list.h"


bool linked_list_check_if_struct_valid(const struct linked_list *list);


struct linked_list *linked_list_create()
{
	struct linked_list *list = (struct linked_list *)malloc(sizeof(struct linked_list));
	if (list == NULL)
	{
		perror("Unable to allocate memory for list struct");
		return NULL;
	}
	
	list->length = 0;
	return list;
}

void linked_list_free(struct linked_list *list)
{
	if (list != NULL)
	{
		if (list->head != NULL)
		{
			struct linked_list_node *current_node = list->head;
			list->head = list->head->next;

			if (current_node->data != NULL)
			{
				free(current_node->data);
				current_node->data = NULL;
			}
			free(current_node);
		}
		free(list);
	}
}

void *linked_list_get(const struct linked_list *list, uint32_t index)
{
	if (linked_list_check_if_struct_valid(list) == false)
	{
		perror("Invlid linked list struct passed to linked_list_get()");
		return NULL;
	}

	if (index >= list->length)
	{
		printf("Warning: requested out of range element in a list\n");
		return NULL;
	}
	
	const struct linked_list_node *current_node = list->head;
	while(index != 0)
	{
		if (current_node == NULL)
		{
			return NULL;
		}

		current_node = current_node->next;
		index--;
	}
	return current_node->data;
}

int linked_list_insert(struct linked_list *list, uint32_t index, void *data, uint32_t data_size)
{
	if (linked_list_check_if_struct_valid(list) == false)
	{
		perror("Invlid linked list struct passed to linked_list_insert()");
		return -1;
	}

	if (index > list->length)
	{
		printf("Warning: requested out of range element in a list\n");
		index = list->length;
	}

	struct linked_list_node *new_node = (struct linked_list_node *)malloc(sizeof(struct linked_list_node));

	if (new_node == NULL)
	{
		perror("Allocation of a new node failed");
		return -1;
	}

	new_node->data_size = data_size;
	new_node->data = (void *)malloc(data_size);
	memcpy(new_node->data, data, data_size);

	if(index == 0)
	{
		struct linked_list_node *node = list->head;
		list->head = new_node;
		list->head->next = node;
	}
	else
	{
		struct linked_list_node *node = linked_list_get(list, index -1);
		if (node == NULL)
		{
			perror("Wrong index in linked_list_insert()");
			free(new_node->data);
			free(new_node);
			return -1;
		}
		new_node->next = node->next;
		node->next = new_node;
	}
	list->length++;
	return 0;
}


void linked_list_remove(struct linked_list *list, uint32_t index)
{
	if (linked_list_check_if_struct_valid(list) == false)
	{
		perror("Invlid linked list struct passed to linked_list_remove()");
		return;
	}

	if (index >= list->length)
	{
		printf("Warning: requested out of range element in a list\n");
		index = list->length - 1;
	}

	if (index == 0)
	{
		struct linked_list_node *node = list->head;
		list->head = list->head->next;
		free(node->data);
		free(node);
	}
	else
	{
		struct linked_list_node *node = linked_list_get(list, index - 1);
		if (node == NULL)
		{
			perror("Wrong index in linked_list_remove()");
			return;
		}
		
		struct linked_list_node *old_node = node->next;
		if (old_node != NULL)
		{
			node->next = old_node->next;
			if (old_node->data != NULL)
			{
				free(old_node->data);
			}
			free(old_node);
		}
	}
	list->length--;
}

int linked_list_push(struct linked_list *list, void *data, uint32_t data_size)
{
	return linked_list_insert(list, 0, data, data_size);	
}

void *linked_list_pop(struct linked_list *list)
{
	struct linked_list_node *node = linked_list_get(list, 0);
	void * buffer = (void *)malloc(node->data_size);
	memcpy(buffer, node->data, node->data_size);
	linked_list_remove(list, 0);
	return buffer;
}

bool linked_list_check_if_struct_valid(const struct linked_list *list)
{
	bool is_valid = (list != NULL);
	return is_valid;
}
