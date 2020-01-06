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
	uint32_t key_size;
	void *value;
	uint32_t value_size;
	struct hashtable_node *next;
};

struct hashtable
{
	uint32_t (*hash_function)(void *, uint32_t);
	bool (*compare_function)(void *, void *, uint32_t);
	uint32_t (*create_node)(struct hashtable_node_list *, void *, uint32_t, void *, uint32_t);
	uint32_t (*free_node)(struct hashtable_node_list *, void *);
	uint32_t max_size;
	uint32_t current_size;
	uint32_t max_node_list_size;
	struct hashtable_node_list *data;
};

struct hashtable *hashtable_create(uint32_t (*hash_function)(const void *, uint32_t),
        bool (*compare_function)(const void *, const void *, uint32_t),
        uint32_t (*create_node)(struct hashtable_node_list *, void *, uint32_t, void *, uint32_t),
        uint32_t (*free_node)(struct hashtable_node_list *, void *, uint32_t),
        uint32_t max_size = HASHTABLE_SIZE_LIMIT,
        uint32_t initial_size = HASHTABLE_DEFAULT_SIZE,
        uint32_t max_node_list_size = HASHTABLE_LIST_LIMIT);
	

void* hashtable_get(const struct hashtable *hashtable, const void *key, uint32_t key_size);

int hashtable_put(struct hashtable *hashtable, const void *key, uint32_t key_size, const void *data, uint32_t data_size);

int hashtable_remove(struct hashtable *hashtable, const void *key, uint32_t key_size);

void hashtable_free(struct hashtable *hashtable);  

#endif
