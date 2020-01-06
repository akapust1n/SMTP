#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

struct hashtable_node
{
	uint32_t key;
	void *value;
};

struct hashtable
{
	uint32_t (*hash_function)(void *);
	uint32_t (*compare_function)(void *, void *);
	uint32_t max_size;
	uint32_t current_size;
	hashtable_node *data;
};

struct hashtable *hashtable_create(uint32_t (*hash_function)(void *), uint32_t (*compare_function)(void *, void *), uint32_t initial_size);
void* hashtable_get(const struct hashtable *hashtable, const void *key);
int hashtable_put(struct hashtable *hashtable, const void *key, const void *data);
int hashtable_remove(struct hashtable *hashtable, const void *key);
void hashtable_free(struct hashtable *hashtable);  

#endif
