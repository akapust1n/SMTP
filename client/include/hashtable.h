#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

// Initial max size (in node lists)
#define HASHTABLE_DEFAULT_SIZE 256

// Total max size (in node lists)
#define HASHTABLE_SIZE_LIMIT 65535

// Maximum nodes in a single node list;
// Reallocate memory and rehash if this value reached
#define HASHTABLE_LIST_LIMIT 32


struct hashtable_node_list
{
	uint32_t list_length;
	struct hashtable_node *list;
};

struct hashtable_node
{
	void *key;
	void *value;
	struct hashtable_node *next;
};

struct hashtable
{
	uint32_t (*hash_function)(void *);
	bool (*compare_function)(void *, void *);
	uint32_t max_size;
	uint32_t current_size;
	uint32_t max_node_list_size;
	struct hashtable_node_list *data;
};

struct hashtable *hashtable_create(uint32_t (*hash_function)(const void *), bool (*compare_function)(const void *, const void *), uint32_t max_size, uint32_t initial_size, uint32_t max_node_list_size);
void* hashtable_get(const struct hashtable *hashtable, const void *key);
int hashtable_put(struct hashtable *hashtable, const void *key, const void *data);
int hashtable_remove(struct hashtable *hashtable, const void *key);
void hashtable_free(struct hashtable *hashtable);  

#endif
