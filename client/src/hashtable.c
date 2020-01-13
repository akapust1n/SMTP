#include "hashtable.h"


struct hashtable *hashtable_rehash(struct hashtable *hashtable, uint32_t new_size);
bool hashtable_check_if_struct_valid(const struct hashtable *hashtable);


struct hashtable *hashtable_create(uint32_t (*hash_function)(const void *, uint32_t),
	bool (*compare_function)(const void *, const void *, uint32_t),
	void *(*create_node)(struct hashtable_node_list *, void *, uint32_t, void *, uint32_t),
        void (*free_node)(struct hashtable_node_list *, void *),
	uint32_t max_size,
	uint32_t initial_size,
	uint32_t max_node_list_size)
{
	if (hash_function == NULL)
	{
		perror("NULL-pointer passed as hash function in hashtable_create()");
		return NULL;
	}
	if (compare_function == NULL)
	{
		perror("NULL-pointer passed as compare function in hashtable_create()");
		return NULL;
	}

	if (create_node == NULL)
	{
		perror("NULL-pointer passed as create node function in hashtable_create()");
		return NULL;
	}
	
	if (free_node == NULL)
	{
		perror("NULL-pointer passed as free node function in hashtable_create()");
		return NULL;
	}
	
	if (initial_size == 0)
	{
		fprintf("Warning: zero size supplied to hashtable_create(). Assuming default value\n");
		initial_size = HASHTABLE_DEFAULT_SIZE;
	}

	
	if (max_size == 0)
	{
		fprintf("Warning: zero size supplied to hashtable_create(). Assuming default value\n");
		initial_size = HASHTABLE_SIZE_LIMIT;
	}

	
	if (max_node_list_size == 0)
	{
		fprintf("Warning: zero size supplied to hashtable_create(). Assuming default value\n");
		initial_size = HASHTABLE_LIST_LIMIT;
	}
	
	struct hashtable *table = (struct hashtable*)malloc(sizeof(struct hashtable));
	if (table == NULL)
	{
		perror("Unable to allocate memory for the hashtable");
		return NULL;
	}

	table->hash_function = hash_function;
	table->compare_function = compare_function;
	table->max_size = max_size;
	table->current_size = initial_size;
	table->max_node_list_size = max_node_list_size;
	table->data = (struct hashtable_node_list *)malloc(initial_size * sizeof(struct hashtable_node_list));

	if (table->data == NULL)
	{
		perror("Unable to allocate memory for hashtable nodes");
		free(table);
		retun NULL;
	}
	
	for (struct hashtable_node_list *node_list = table->data; node_list != table->data + table->current_size; node_list++)
	{
		node_list->length = 0;
		node_list->list = NULL;
	}
	
	return table;
}

void* hashtable_get(const struct hashtable *hashtable, const void *key, uint32_t key_size)
{
	if (key == NULL)
	{
		perror("Key is NULL in hashtable_get()");
		return NULL;
	}

	if (hashtable_check_if_struct_valid(hashtable) == false)
	{
		perror("Hashtable validity check failed");
		return NULL;
	}
	
	uint32_t key_hashed = hashtable->hash_function(key) % hashtable->current_size;

	const struct hashtable_node_list *node_list = hashtable->data[key_hashed];

	if (node_list->length == 0)
	{
		return NULL;
	}
	
	for (const struct hashtable_node *node = node_list->list; node != NULL; node = node->next;)
	{
		if (hashtable->compare_function(node->key, key))
		{
			return node->value;
		}
	}
	
	return NULL;
}

struct hashtable_node_list* hashtable_get_list(const struct hashtable *hashtable, const void *key, uint32_t key_size)
{
	if (key == NULL)
	{
		perror("Key is NULL in hashtable_get()");
		return NULL;
	}

	if (hashtable_check_if_struct_valid(hashtable) == false)
	{
		perror("Hashtable validity check failed");
		return NULL;
	}
	
	uint32_t key_hashed = hashtable->hash_function(key, key_size) % hashtable->current_size;

	const struct hashtable_node_list *node_list = hashtable->data[key_hashed];

	return node_list;
}

int hashtable_put(struct hashtable *hashtable, const void *key, uint32_t key_size, const void *data, uint32_t data_size)
{
	if (key == NULL)
	{
		perror("Key is NULL in hashtable_put()");
		return -1;
	}
	
	if (data == NULL)
	{
		printf("Warning: data is NULL in hashtable_put()");
	}

	if (hashtable_check_if_struct_valid(hashtable) == false)
	{
		perror("Hashtable validity check failed");
		return -1;
	}

	uint32_t key_hashed = hashtable->hash_function(key, key_size) % hashtable->current_size;

	struct hashtable_node_list *node_list = hashtable->data[key_hashed];

	if (node_list->length >= hashtable->max_node_list_size)
	{
		if (hashtable->current_size == hashtable->max_size)
		{
			perror("Max size for hashtable reached. Unable to rehash");
			return -1;
		}
		uint32_t new_size = (((hashtable->current_size << 1) < hashtable->max_size) &&
				((hashtable->current_size << 1) > hashtable->current_size))
				? hashtable->current_size << 1 : hashtable->max_size;
		if (hashtable_rehash(hashtable, new_size) == NULL)
		{
			perror("Failed to rehash hashtable");
			return -1;
		}
	}
	hashtable->create_node(node_list, key, key_size, data, data_size);
}

int hashtable_remove(struct hashtable *hashtable, const void *key, uint32_t key_size)
{
	if (key == NULL)
	{
		perror("Key is NULL in hashtable_remove()");
		return -1;
	}

	if (hashtable_check_if_struct_valid(hashtable) == false)
	{
		perror("Hashtable validity check failed");
		return -1;
	}

	uint32_t key_hashed = hashtable->hash_function(key, key_size) % hashtable->current_size;

	struct hashtable_node_list *node_list = hashtable->data[key_hashed];

	return hashtable->free_node(node_list, key, key_size);
}

void hashtable_free(struct hashtable *hashtable)
{
	if (hashtable->data != NULL)
	{
		for (struct hashtable_node_list *node_list = table->data; node_list != table->data + table->current_size; node_list++)
		{
			struct hashtable_node *current_node = node_list->list;
			while(current_node != NULL)
			{
				hashtable->free_node(node_list, current_node);
				current_node = node_list->list;
			}
		}
		free(hashtable->data);
		hashtable->data = NULL;
	}
	free(hashtable);
}

bool hashtable_check_if_struct_valid(const struct hashtable *hashtable)
{
	bool is_valid = ((hashtable != NULL) &&
	(hashtable->hash_function != NULL) &&
	(hashtable->compare_function != NULL) &&
	(hashtable->create_node != NULL) &&
	(hashtable->free_node != NULL) &&
	(hashtable->max_size > 0) &&
	(hashtable->current_size > 0) &&
	(hashtable->current_size <= hashtable->max_size) &&
	(hashtable->max_node_list_size > 0) &&
	(hashtable->data != NULL));

	return is_valid;
}

struct hashtable *hashtable_rehash(struct hashtable *hashtable, uint32_t new_size)
{
	if ((new_size == 0) || (hashtable->max_size < new_size))
	{
		perror("Bad value for new_size in rehash funcion");
		return NULL;
	}
	
	if (hashtable_check_if_struct_valid(hashtable) == false)
	{
		perror("Passed invalid hashtable struct to rehash function");
		retun NULL;
	}

	struct hashtable_node_list *new_node_list = (hashtable_node_list *)malloc(new_size * sizeof(hashtable_node_list));
	if (new_node_list == NULL)
	{
		perror("Failed to allocate memory for rehashing");
		return NULL;
	}
	struct hashtable_node_list *old_node_list = hashtable->data;
	uint32_t old_list_size = hashtable->current_size;

	hashtable->data = new_node_list;
	hashtable->current_size = new_size;

	for (struct hashtable_node_list *node_list = old_node_list; node_list != old_node_list + old_size; node_list++)
	{
		struct hashtable_node *current_node = node_list->list;
		while(current_node != NULL)
		{
			hashtable->create_node(hashtable->data, current_node->key, current_node->key_size, current_node->data, current_node->data_size);
			hashtable->free_node(node_list, current_node);
			current_node = node_list->list;
		}
	}
	free(old_node_list);
	return hashtable;
}
