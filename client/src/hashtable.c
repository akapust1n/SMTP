#include "hashtable.h"

struct hashtable *hashtable_rehash(struct hashtable *hashtable, uint32_t new_size);


struct hashtable *hashtable_create(uint32_t (*hash_function)(const void *), uint32_t (*compare_function)(const void *, const void *),
	uint32_t max_size = HASHTABLE_SIZE_LIMIT,
	uint32_t initial_size = HASHTABLE_DEFAULT_SIZE,
	uint32_t max_node_list_size = HASHTABLE_LIST_LIMIT)
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

void* hashtable_get(const struct hashtable *hashtable, const void *key)
{
	if (hashtable == NULL)
	{
		perror("NULL-ptr passed down as hashtable pointer to hashtable_get()");
		return NULL;
	}
	if (key == NULL)
	{
		perror("Key is NULL in hashtable_get()\n");
		return NULL;
	}

	if (hashtable->hash_function == NULL)
	{
		perror("NULL-pointer passed as hash function in hashtable_get()");
		return NULL;
	}	

	if (hashtable->compare_function == NULL)
	{
		perror("NULL-pointer passed as compare function in hashtable_get()");
		return NULL;
	}
	
	if (hashtable->data == NULL)
	{
		perror("Hashtable data is NULL");
		return NULL;
	}

	uint32_t key_hashed = hashtable->hash_function(key);
	void * result = NULL;

	const struct hashtable_node_list *node_list = hashtable->data[key_hashed % hashtable->max_size];
	
	for (const struct hashtable_node *node = node_list
	if ((node->in_use == false) && hashtable->compare_function(node->key, key) == 0))
	{
		result = i->value;
	}
	
	return result;
}

int hashtable_put(struct hashtable *hashtable, const void *key, const void *data)
{
	if (hashtable == NULL)
	{
		perror("NULL-ptr passed down as hashtable pointer to hashtable_put()");
		return -1;
	}

	if (hashtable->hash_function == NULL)
	{
		perror("NULL-pointer passed as hash function in hashtable_put()");
		return -1;
	}

	if (hashtable->compare_function == NULL)
	{
		perror("NULL-pointer passed as compare function in hashtable_put()");
		return -1;
	}

	if (key == NULL)
	{
		perror("Key is NULL in hashtable_put()");
		return -1;
	}
	
	if (data == NULL)
	{
		printf("Warning: data is NULL in hashtable_put()");
	}
	
	if (hashtable->data == NULL)
	{
		perror("Hashtable data is NULL in hashtable_put()");
		return -1;
	}

	uint32_t key_hashed = hashtable->hash_function(key);

	for (const struct hashtable_node* i = hashtable->data; i != hashtable->data + hashtable->max_size; i++)
	{
		if ((i->in_use == false) || (i->key_hashed != key_hashed))
		{
			continue;
		}
		
		if (hashtable->compare_function(i->key, key) == 0)
		{
			result = i->value;
		}
	}
}

int hashtable_remove(struct hashtable *hashtable, const void *key);
void hashtable_free(struct hashtable *hashtable); 
