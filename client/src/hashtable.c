#include "hashtable.h"

struct hashtable *hashtable_rehash(struct hashtable *hashtable, uint32_t new_size);


struct hashtable *hashtable_create(uint32_t (*hash_function)(void *), uint32_t (*compare_function)(void *, void *),uint32_t initial_size = HASHTABLE_DEFAULT_SIZE)
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
	
	struct hashtable *table = (struct hashtable*)malloc(sizeof(struct hashtable));
	if (table == NULL)
	{
		perror("Unable to allocate memory for the hashtable");
		return NULL;
	}

	table->hash_function = hash_function;
	table->compare_function = compare_function;
	table->max_size = initial_size;
	table->current_size = 0;
	table->data = NULL;
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
		printf("Warning: key is NULL in hashtable_get()\n");
		return NULL;
	}
	uint32_t key_hashed = hashtable->hash_function(key);
}

int hashtable_put(struct hashtable *hashtable, const void *key, const void *data);
int hashtable_remove(struct hashtable *hashtable, const void *key);
void hashtable_free(struct hashtable *hashtable); 
